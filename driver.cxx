
#include <emmintrin.h>
#include <smmintrin.h>
#include <immintrin.h>

#include <iterator>
#include <vector>
#include <iomanip>
#include <cctype>
#include <ostream>
#include <map>
#include <algorithm>
#include <boost/timer/timer.hpp>
#include <iostream>
#include <boost/range.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/function.hpp>
#include <boost/range/any_range.hpp>
#include <boost/bind/bind.hpp>
#include <boost/iterator/indirect_iterator.hpp>
#include <boost/shared_container_iterator.hpp>
#include <boost/mem_fn.hpp>
#include <boost/log/trivial.hpp>
#include <boost/timer/timer.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/foreach.hpp>




struct NamedFunction{
        NamedFunction(std::string const& name, boost::function<unsigned()> f)
                :name_(name),
                f_(f)
        {}
        std::string const& Name()const{ return name_; }
        unsigned Execute()const{
                return f_();
        }
private:
        std::string name_;
        boost::function<unsigned()> f_;
};
typedef boost::any_range<
        NamedFunction
        , boost::forward_traversal_tag
        , NamedFunction 
        , std::ptrdiff_t
> FunctionRange;
struct Test{
        virtual ~Test(){}
        virtual std::string Name()const=0;
        virtual FunctionRange Subs()const=0;
};
#define ADD_PROFILE(NAME) \
        builder.Add(#NAME , boost::bind(boost::mem_fn(&self_type::NAME          ), this));
struct SubBuilder{
        typedef std::vector<NamedFunction>    VectorType;
        typedef boost::shared_ptr<VectorType> SharedVectorType;
        SubBuilder(){
                Restart();
        }
        void Restart(){
                vec_ = boost::make_shared<VectorType>();
        }
        template<class F>
        void Add(std::string const& name, F const& f){
                vec_->push_back(NamedFunction(name, f));
        }
        FunctionRange Make(){
                SharedVectorType V = vec_;
                Restart();
                return FunctionRange(boost::make_shared_container_iterator(V->begin(), V),
                                     boost::make_shared_container_iterator(V->end(), V));
        }
private:
        boost::shared_ptr<std::vector<NamedFunction> > vec_;
};
namespace Detail{
        template<class _FwdIt> inline
	_FwdIt _Max_element(_FwdIt _First, _FwdIt _Last)
	{	// find largest element, using operator<
	_FwdIt _Found = _First;
	if (_First != _Last)
		for (; ++_First != _Last; )
			if (*_Found < *_First)
				_Found = _First;
	return (_Found);
	}
        template<class T>
        T _Max(T left, T right){
                return ( left < right ? right : left );
        }
} // end namespace Detail

struct MaxElement : Test{
        typedef MaxElement self_type;
        explicit MaxElement(int n){
                vec_.resize(n);
                for(size_t idx=0;idx!=n;++idx){
                        vec_[idx] = (idx * idx )  % 762 + ( idx * 34 ) % 493  + idx / 1000;
                }
                size_t sz = n * sizeof(unsigned);
                alligned_vec_ = (unsigned*)_mm_malloc(sz, 16);
                std::memcpy(alligned_vec_, &vec_[0], sz);
        }
        ~MaxElement(){
                _mm_free(alligned_vec_);
        }
        unsigned ExecuteStl()const{
                return * Detail::_Max_element(vec_.begin(), vec_.end());
        }
        unsigned ExecuteStlRaw()const{
                return * Detail::_Max_element(&vec_[0], &vec_[0] + vec_.size());
        }
        unsigned ExecuteNaive()const{
                unsigned working = vec_[0];
                for(size_t idx=0;idx!=vec_.size();++idx){
                        working = Detail::_Max(working, vec_[idx]);
                }
                return working;
        }
        unsigned ExecuteNaiveRaw()const{
                unsigned const* first = &vec_[0];
                unsigned const* last  = first + vec_.size();
                unsigned working = *first;
                ++first;
                for(;first!=last;++first){
                        working = Detail::_Max(working, *first);
                }
                return working;
        }
        unsigned ExecuteStride()const{
                enum{ Stride = 4 };
                unsigned working = vec_[0];
                size_t idx=0;
                for(;idx + Stride < vec_.size();idx+=Stride){
                        unsigned A = Detail::_Max(vec_[idx+0], vec_[idx+1]);
                        unsigned B = Detail::_Max(vec_[idx+2], vec_[idx+3]);
                        
                        unsigned E = Detail::_Max(A,B);
                        
                        working = Detail::_Max(working, E);
                }
                for(;idx!=vec_.size();++idx){
                        working = Detail::_Max(working, vec_[idx]);
                }
                return working;
        }
        unsigned ExecuteStrideRaw()const{
                enum{ Stride = 4 };
                unsigned const* first = &vec_[0];
                unsigned const* last  = first + vec_.size();
                unsigned working = *first;
                ++first;
                for(;first + Stride < last;first+=Stride){
                        unsigned A = Detail::_Max(*(first+0), *(first+1));
                        unsigned B = Detail::_Max(*(first+2), *(first+3));
                        
                        unsigned E = Detail::_Max(A,B);
                        
                        working = Detail::_Max(working, E);
                }

                for(;first!=last;++first){
                        working = Detail::_Max(working, *first);
                }
                return working;
        }
        unsigned ExecuteSSE()const{
                enum{ Stride = 4 };
                unsigned const* first = &vec_[0];
                unsigned const* last  = first + vec_.size();
                __m128i M = _mm_set_epi32(*(first+0),
                                          *(first+1),
                                          *(first+2),
                                          *(first+3));
                for(;first + Stride < last;first+=Stride){
                        __m128i A = _mm_set_epi32(*(first+0),
                                                  *(first+1),
                                                  *(first+2),
                                                  *(first+3));
                        M = _mm_max_epu32(A,M);
                }
                unsigned working = Detail::_Max(
                        Detail::_Max( _mm_extract_epi32(M, 0), _mm_extract_epi32(M, 1)),
                        Detail::_Max( _mm_extract_epi32(M, 2), _mm_extract_epi32(M, 3))
                );
                for(;first!=last;++first){
                        working = Detail::_Max(working, *first);
                }
                return working;
        }
        unsigned ExecuteSSELoad()const{
                enum{ Stride = 4 };
                unsigned const* first = &vec_[0];
                unsigned const* last  = first + vec_.size();

                __m128i M = _mm_load_si128((__m128i const*)first);
                for(;first + Stride < last;first+=Stride){
                        __m128i A = _mm_load_si128((__m128i const*)first);
                        M = _mm_max_epu32(A,M);
                }
                unsigned working = Detail::_Max(
                        Detail::_Max( _mm_extract_epi32(M, 0), _mm_extract_epi32(M, 1)),
                        Detail::_Max( _mm_extract_epi32(M, 2), _mm_extract_epi32(M, 3))
                );
                for(;first!=last;++first){
                        working = Detail::_Max(working, *first);
                }
                return working;
        }
        unsigned ExecuteSSELoadOneReg()const{
                enum{ Stride = 4 };
                unsigned const* first = &vec_[0];
                unsigned const* last  = first + vec_.size();

                __m128i M = _mm_load_si128((__m128i const*)first);
                for(;first + Stride < last;first+=Stride){
                        M = _mm_max_epu32(_mm_load_si128((__m128i const*)first),M);
                }
                unsigned working = Detail::_Max(
                        Detail::_Max( _mm_extract_epi32(M, 0), _mm_extract_epi32(M, 1)),
                        Detail::_Max( _mm_extract_epi32(M, 2), _mm_extract_epi32(M, 3))
                );
                for(;first!=last;++first){
                        working = Detail::_Max(working, *first);
                }
                return working;
        }
        unsigned ExecuteSSEStrideRaw()const{
                enum{ Stride = 16 };
                unsigned const* first = &vec_[0];
                unsigned const* last  = first + vec_.size();
                __m128i M = _mm_set_epi32(*(first+0),
                                          *(first+1),
                                          *(first+2),
                                          *(first+3));
                for(;first + Stride < last;){
                        __m128i A = _mm_set_epi32(*(first+0),
                                                  *(first+1),
                                                  *(first+2),
                                                  *(first+3));
                        first += Stride;
                        __m128i B = _mm_set_epi32(*(first+0),
                                                  *(first+1),
                                                  *(first+2),
                                                  *(first+3));
                        first += Stride;
                        __m128i C = _mm_set_epi32(*(first+0),
                                                  *(first+1),
                                                  *(first+2),
                                                  *(first+3));
                        first += Stride;
                        __m128i D = _mm_set_epi32(*(first+0),
                                                  *(first+1),
                                                  *(first+2),
                                                  *(first+3));
                        
                        __m128i E = _mm_max_epu32(A,B);
                        __m128i F = _mm_max_epu32(C,D);

                        __m128i G = _mm_max_epu32(E,F);

                        M = _mm_max_epu32(A,M);
                }
                unsigned working = Detail::_Max(
                        Detail::_Max( _mm_extract_epi32(M, 0), _mm_extract_epi32(M, 1)),
                        Detail::_Max( _mm_extract_epi32(M, 2), _mm_extract_epi32(M, 3))
                );
                for(;first!=last;++first){
                        working = Detail::_Max(working, *first);
                }
                return working;
        }
        unsigned ExecuteSSEStide()const{
                __m128i M = _mm_set_epi32(vec_[0], vec_[1], vec_[2], vec_[3]);
                enum{ Stride = 16 };
                size_t idx=0;
                for(;idx + Stride < vec_.size();idx+=Stride){
                        __m128i A = _mm_set_epi32(vec_[idx+ 0+0], vec_[idx+ 0+1], vec_[idx+ 0+2], vec_[idx+ 0+3]);
                        __m128i B = _mm_set_epi32(vec_[idx+ 4+0], vec_[idx+ 4+1], vec_[idx+ 4+2], vec_[idx+ 4+3]);
                        __m128i C = _mm_set_epi32(vec_[idx+ 8+0], vec_[idx+ 8+1], vec_[idx+ 8+2], vec_[idx+ 8+3]);
                        __m128i D = _mm_set_epi32(vec_[idx+12+0], vec_[idx+12+1], vec_[idx+12+2], vec_[idx+12+3]);

                        __m128i E = _mm_max_epu32(A,B);
                        __m128i F = _mm_max_epu32(C,D);

                        __m128i G = _mm_max_epu32(E,F);

                        M = _mm_max_epu32(A,M);
                }
                unsigned working = Detail::_Max(
                        Detail::_Max( _mm_extract_epi32(M, 0), _mm_extract_epi32(M, 1)),
                        Detail::_Max( _mm_extract_epi32(M, 2), _mm_extract_epi32(M, 3))
                );

                for(;idx!=vec_.size();++idx){
                        working = Detail::_Max(working, vec_[idx]);
                }
                return working;
        }
        unsigned ExecuteBoostRange()const{
                return *boost::max_element(vec_);
        }
        
        virtual FunctionRange Subs()const{
                SubBuilder builder;
                ADD_PROFILE(ExecuteStl);
                ADD_PROFILE(ExecuteStlRaw);
                ADD_PROFILE(ExecuteNaive);
                ADD_PROFILE(ExecuteNaiveRaw);
                ADD_PROFILE(ExecuteStride);
                ADD_PROFILE(ExecuteStrideRaw);
                ADD_PROFILE(ExecuteSSE);
                ADD_PROFILE(ExecuteSSELoad);
                ADD_PROFILE(ExecuteSSELoadOneReg);
                ADD_PROFILE(ExecuteSSEStide);
                ADD_PROFILE(ExecuteBoostRange);
                ADD_PROFILE(ExecuteSSEStrideRaw);

                return builder.Make();
        }
        virtual std::string Name()const{ return "MaxElement"; }
private:
        std::vector<unsigned> vec_;
        unsigned* alligned_vec_;
};

struct CountDisjoint : Test{
        typedef CountDisjoint self_type;
        explicit CountDisjoint(int n){
                vec_.resize(n);
                for(size_t idx=0;idx!=n;++idx){
                        vec_[idx] = (idx * idx )  % 762 + ( idx * 34 ) % 493  + idx / 1000 + ( idx % 5 == 0 ? 1 : 0 );
                }
                size_t sz = n * sizeof(unsigned);
                alligned_vec_ = (unsigned*)_mm_malloc(sz, 16);
                std::memcpy(alligned_vec_, &vec_[0], sz);
        }
        unsigned ExecuteNaive()const{
                unsigned mask = 0x4587au;

                unsigned const* first = &vec_[0];
                unsigned const* last  = first + vec_.size();
                unsigned count = 0;
                for(;first!=last;++first){
                        if( ( *first & mask ) == 0 )
                                ++count;
                }
                return count;
        }
        unsigned ExecuteNaiveUnrolled()const{
                unsigned mask = 0x4587au;

                unsigned const* first = &vec_[0];
                unsigned const* last  = first + vec_.size();
                unsigned count = 0;
                enum{ Stride = 8 };
                for(;first + Stride < last;){

                        if( ( *first++ & mask ) == 0 ) ++count;
                        if( ( *first++ & mask ) == 0 ) ++count;
                        if( ( *first++ & mask ) == 0 ) ++count;
                        if( ( *first++ & mask ) == 0 ) ++count;
                        if( ( *first++ & mask ) == 0 ) ++count;

                        if( ( *first++ & mask ) == 0 ) ++count;
                        if( ( *first++ & mask ) == 0 ) ++count;
                        if( ( *first++ & mask ) == 0 ) ++count;
                }
                for(;first!=last;++first){
                        if( ( *first & mask ) == 0 )
                                ++count;
                }
                return count;
        }
        unsigned ExecuteSSE()const{
                unsigned mask = 0x4587au;

                unsigned const* first = &vec_[0];
                unsigned const* last  = first + vec_.size();
                enum{ Stride = 4 };

                __m128i M       = _mm_set1_epi32(mask);
                __m128i Counter = _mm_setzero_si128();
                __m128i Mask    = _mm_set1_epi32(1);

                for(;first + Stride < last;first+=Stride){
                        // load into A = [v0,v1,v2,v3]
                        __m128i A = _mm_load_si128((__m128i const*)first);
                        // bit and with mask [v0,v1,v2,v3] & [mask,mask,mask,mask]
                        //                 = [v0 & mask, ... ]
                        __m128i B = _mm_and_si128(M, A);
                        // now compare with zero,  maps to {0,~0}
                        __m128i C = _mm_cmpeq_epi32(B, _mm_setzero_si128());
                        // map ~0 to 1, which corresponds to [c0,c1,c2,c3}, where ci \in {0,1}
                        __m128i D = _mm_and_si128(C,Mask);
                        // add to the summing sum 
                        Counter = _mm_add_epi32(D, Counter);
                }
                unsigned count = 0;
                count += _mm_extract_epi32(Counter, 0);
                count += _mm_extract_epi32(Counter, 1);
                count += _mm_extract_epi32(Counter, 2);
                count += _mm_extract_epi32(Counter, 3);
                for(;first!=last;++first){
                        if( ( *first & mask ) == 0 )
                                ++count;
                }
                return count;
        }
        
        unsigned ExecuteSSEStream()const{
                unsigned mask = 0x4587au;

                unsigned const* first = &vec_[0];
                unsigned const* last  = first + vec_.size();
                enum{ Stride = 4 };

                __m128i M       = _mm_set1_epi32(mask);
                __m128i Counter = _mm_setzero_si128();
                __m128i Mask    = _mm_set1_epi32(1);

                for(;first + Stride < last;first+=Stride){
                        __m128i A = _mm_stream_load_si128((__m128i*)first);
                        __m128i B = _mm_and_si128(M, A);
                        __m128i C = _mm_and_si128(_mm_cmpeq_epi32(B, _mm_setzero_si128()),Mask);
                        Counter = _mm_add_epi32(C, Counter);
                }
                unsigned count = 0;
                count += _mm_extract_epi32(Counter, 0);
                count += _mm_extract_epi32(Counter, 1);
                count += _mm_extract_epi32(Counter, 2);
                count += _mm_extract_epi32(Counter, 3);
                for(;first!=last;++first){
                        if( ( *first & mask ) == 0 )
                                ++count;
                }
                return count;
        }
        virtual std::string Name()const{ return "CountDisjoint"; }
        
        virtual FunctionRange Subs()const{
                SubBuilder builder;
                ADD_PROFILE(ExecuteNaive);
                ADD_PROFILE(ExecuteNaiveUnrolled);
                ADD_PROFILE(ExecuteSSE);
                ADD_PROFILE(ExecuteSSEStream);

                return builder.Make();
        }
private:
        std::vector<unsigned> vec_;
        unsigned* alligned_vec_;
};

int main(){
        enum{ Dp = 8 };
        enum{ Count = 30 };
        enum{ Width = 50 };
        std::vector<boost::shared_ptr<Test> > T;
        //T.push_back(boost::make_shared<MaxElement>(100000000));
        T.push_back(boost::make_shared<CountDisjoint>(100000000));

        BOOST_FOREACH(boost::shared_ptr<Test> test, T){
                std::map<std::string, boost::timer::nanosecond_type> profile;
                for(size_t idx=0;idx!=Count;++idx){
                        BOOST_LOG_TRIVIAL(trace) << test->Name();
                        BOOST_FOREACH(NamedFunction const& F, test->Subs()){
                                boost::timer::cpu_timer tmr;
                                unsigned result = F.Execute();
                                std::string token = test->Name() + "." + F.Name();
                                profile[token] += tmr.elapsed().wall;
                                BOOST_LOG_TRIVIAL(trace) << std::left << std::setw(Width) << token << boost::timer::format(tmr.elapsed(), Dp, " took %w seconds") << " ( " << result << ")";
                        }
                }
                typedef std::pair<std::string, boost::timer::nanosecond_type>  pair_type;
                std::vector<pair_type> view(profile.begin(), profile.end());
                struct sorter{
                        bool operator()(pair_type const& l, pair_type const& r)const{
                                return l.second > r.second;
                        }
                };
                boost::sort(view, sorter());
                BOOST_FOREACH(pair_type& p, view){
                        p.second /= Count;
                        BOOST_LOG_TRIVIAL(trace) << std::left << std::setw(Width) << p.first << std::setprecision(Dp) << (p.second / 1000000000.0);
                }
        }
}

