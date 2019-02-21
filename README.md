personal code trying to figure out optimial SSE execution.
I started this because I wanted to find the quickest way to count the number of disjoint bitmask in a vector

        ./driver
        [2019-02-21 17:26:28.953376] [0x00004790] [trace]   CountDisjoint
        [2019-02-21 17:26:29.051416] [0x00004790] [trace]   CountDisjoint.ExecuteNaive                         took 0.09723496 seconds ( 426471)
        [2019-02-21 17:26:29.117442] [0x00004790] [trace]   CountDisjoint.ExecuteNaiveUnrolled                 took 0.06627453 seconds ( 426471)
        [2019-02-21 17:26:29.164461] [0x00004790] [trace]   CountDisjoint.ExecuteSSE                           took 0.04654257 seconds ( 426471)
        [2019-02-21 17:26:29.207478] [0x00004790] [trace]   CountDisjoint.ExecuteSSEStream                     took 0.04355411 seconds ( 426471)
        [2019-02-21 17:26:29.260499] [0x00004790] [trace]   CountDisjoint.ExecuteSSEPopcount                   took 0.05242222 seconds ( 426471)
        [2019-02-21 17:26:29.260499] [0x00004790] [trace]   CountDisjoint
        [2019-02-21 17:26:29.361540] [0x00004790] [trace]   CountDisjoint.ExecuteNaive                         took 0.10105877 seconds ( 426471)
        [2019-02-21 17:26:29.426566] [0x00004790] [trace]   CountDisjoint.ExecuteNaiveUnrolled                 took 0.06477199 seconds ( 426471)
        [2019-02-21 17:26:29.470583] [0x00004790] [trace]   CountDisjoint.ExecuteSSE                           took 0.04403312 seconds ( 426471)
        [2019-02-21 17:26:29.513600] [0x00004790] [trace]   CountDisjoint.ExecuteSSEStream                     took 0.04330580 seconds ( 426471)
        [2019-02-21 17:26:29.564621] [0x00004790] [trace]   CountDisjoint.ExecuteSSEPopcount                   took 0.05071537 seconds ( 426471)
        [2019-02-21 17:26:29.564621] [0x00004790] [trace]   CountDisjoint
        [2019-02-21 17:26:29.661660] [0x00004790] [trace]   CountDisjoint.ExecuteNaive                         took 0.09676231 seconds ( 426471)
        [2019-02-21 17:26:29.724685] [0x00004790] [trace]   CountDisjoint.ExecuteNaiveUnrolled                 took 0.06266775 seconds ( 426471)
        [2019-02-21 17:26:29.766702] [0x00004790] [trace]   CountDisjoint.ExecuteSSE                           took 0.04256284 seconds ( 426471)
        [2019-02-21 17:26:29.809719] [0x00004790] [trace]   CountDisjoint.ExecuteSSEStream                     took 0.04302475 seconds ( 426471)
        [2019-02-21 17:26:29.862740] [0x00004790] [trace]   CountDisjoint.ExecuteSSEPopcount                   took 0.05301708 seconds ( 426471)
        [2019-02-21 17:26:29.862740] [0x00004790] [trace]   CountDisjoint
        [2019-02-21 17:26:29.960779] [0x00004790] [trace]   CountDisjoint.ExecuteNaive                         took 0.09774526 seconds ( 426471)
        [2019-02-21 17:26:30.047814] [0x00004790] [trace]   CountDisjoint.ExecuteNaiveUnrolled                 took 0.08652704 seconds ( 426471)
        [2019-02-21 17:26:30.110839] [0x00004790] [trace]   CountDisjoint.ExecuteSSE                           took 0.06270685 seconds ( 426471)
        [2019-02-21 17:26:30.160859] [0x00004790] [trace]   CountDisjoint.ExecuteSSEStream                     took 0.05016401 seconds ( 426471)
        [2019-02-21 17:26:30.215881] [0x00004790] [trace]   CountDisjoint.ExecuteSSEPopcount                   took 0.05558274 seconds ( 426471)
        [2019-02-21 17:26:30.215881] [0x00004790] [trace]   CountDisjoint
        [2019-02-21 17:26:30.315921] [0x00004790] [trace]   CountDisjoint.ExecuteNaive                         took 0.09979671 seconds ( 426471)
        [2019-02-21 17:26:30.382948] [0x00004790] [trace]   CountDisjoint.ExecuteNaiveUnrolled                 took 0.06665627 seconds ( 426471)
        [2019-02-21 17:26:30.431968] [0x00004790] [trace]   CountDisjoint.ExecuteSSE                           took 0.04852510 seconds ( 426471)
        [2019-02-21 17:26:30.480987] [0x00004790] [trace]   CountDisjoint.ExecuteSSEStream                     took 0.04894155 seconds ( 426471)
        [2019-02-21 17:26:30.539010] [0x00004790] [trace]   CountDisjoint.ExecuteSSEPopcount                   took 0.05875694 seconds ( 426471)
        [2019-02-21 17:26:30.540011] [0x00004790] [trace]   CountDisjoint.ExecuteNaive                        0.098498584
        [2019-02-21 17:26:30.540011] [0x00004790] [trace]   CountDisjoint.ExecuteNaiveUnrolled                0.069359377
        [2019-02-21 17:26:30.540011] [0x00004790] [trace]   CountDisjoint.ExecuteSSEPopcount                  0.054075212
        [2019-02-21 17:26:30.540011] [0x00004790] [trace]   CountDisjoint.ExecuteSSE                          0.048847897
        [2019-02-21 17:26:30.540011] [0x00004790] [trace]   CountDisjoint.ExecuteSSEStream                    0.045773996
        [2019-02-21 17:26:30.540011] [0x00004790] [trace]   CountDisjoint64
        [2019-02-21 17:26:30.722084] [0x00004790] [trace]   CountDisjoint64.ExecuteNaive                       took 0.18218273 seconds ( 1521107)
        [2019-02-21 17:26:30.884148] [0x00004790] [trace]   CountDisjoint64.ExecuteNaiveUnrolled               took 0.16268002 seconds ( 1521107)
        [2019-02-21 17:26:31.011199] [0x00004790] [trace]   CountDisjoint64.ExecuteSSE128                      took 0.12712740 seconds ( 1521107)
        [2019-02-21 17:26:31.138250] [0x00004790] [trace]   CountDisjoint64.ExecuteSSE128WithZero              took 0.12690207 seconds ( 1521107)
        [2019-02-21 17:26:31.262300] [0x00004790] [trace]   CountDisjoint64.ExecuteSSE128Unrolled              took 0.12331435 seconds ( 1521107)
        [2019-02-21 17:26:31.383348] [0x00004790] [trace]   CountDisjoint64.ExecuteSSE128UnrolledStream        took 0.12141540 seconds ( 1521107)
        [2019-02-21 17:26:31.383348] [0x00004790] [trace]   CountDisjoint64
        [2019-02-21 17:26:31.556417] [0x00004790] [trace]   CountDisjoint64.ExecuteNaive                       took 0.17227789 seconds ( 1521107)
        [2019-02-21 17:26:31.704476] [0x00004790] [trace]   CountDisjoint64.ExecuteNaiveUnrolled               took 0.14810430 seconds ( 1521107)
        [2019-02-21 17:26:31.808518] [0x00004790] [trace]   CountDisjoint64.ExecuteSSE128                      took 0.10418556 seconds ( 1521107)
        [2019-02-21 17:26:31.911559] [0x00004790] [trace]   CountDisjoint64.ExecuteSSE128WithZero              took 0.10306623 seconds ( 1521107)
        [2019-02-21 17:26:32.012600] [0x00004790] [trace]   CountDisjoint64.ExecuteSSE128Unrolled              took 0.10054994 seconds ( 1521107)
        [2019-02-21 17:26:32.116641] [0x00004790] [trace]   CountDisjoint64.ExecuteSSE128UnrolledStream        took 0.10454384 seconds ( 1521107)
        [2019-02-21 17:26:32.116641] [0x00004790] [trace]   CountDisjoint64
        [2019-02-21 17:26:32.279706] [0x00004790] [trace]   CountDisjoint64.ExecuteNaive                       took 0.16287113 seconds ( 1521107)
        [2019-02-21 17:26:32.428766] [0x00004790] [trace]   CountDisjoint64.ExecuteNaiveUnrolled               took 0.14874462 seconds ( 1521107)
        [2019-02-21 17:26:32.533808] [0x00004790] [trace]   CountDisjoint64.ExecuteSSE128                      took 0.10455850 seconds ( 1521107)
        [2019-02-21 17:26:32.636849] [0x00004790] [trace]   CountDisjoint64.ExecuteSSE128WithZero              took 0.10355893 seconds ( 1521107)
        [2019-02-21 17:26:32.737890] [0x00004790] [trace]   CountDisjoint64.ExecuteSSE128Unrolled              took 0.10048297 seconds ( 1521107)
        [2019-02-21 17:26:32.837930] [0x00004790] [trace]   CountDisjoint64.ExecuteSSE128UnrolledStream        took 0.10047027 seconds ( 1521107)
        [2019-02-21 17:26:32.837930] [0x00004790] [trace]   CountDisjoint64
        [2019-02-21 17:26:33.000995] [0x00004790] [trace]   CountDisjoint64.ExecuteNaive                       took 0.16260230 seconds ( 1521107)
        [2019-02-21 17:26:33.151055] [0x00004790] [trace]   CountDisjoint64.ExecuteNaiveUnrolled               took 0.14944114 seconds ( 1521107)
        [2019-02-21 17:26:33.258098] [0x00004790] [trace]   CountDisjoint64.ExecuteSSE128                      took 0.10745703 seconds ( 1521107)
        [2019-02-21 17:26:33.360138] [0x00004790] [trace]   CountDisjoint64.ExecuteSSE128WithZero              took 0.10181835 seconds ( 1521107)
        [2019-02-21 17:26:33.456177] [0x00004790] [trace]   CountDisjoint64.ExecuteSSE128Unrolled              took 0.09592696 seconds ( 1521107)
        [2019-02-21 17:26:33.552215] [0x00004790] [trace]   CountDisjoint64.ExecuteSSE128UnrolledStream        took 0.09554229 seconds ( 1521107)
        [2019-02-21 17:26:33.552215] [0x00004790] [trace]   CountDisjoint64
        [2019-02-21 17:26:33.705276] [0x00004790] [trace]   CountDisjoint64.ExecuteNaive                       took 0.15354453 seconds ( 1521107)
        [2019-02-21 17:26:33.855336] [0x00004790] [trace]   CountDisjoint64.ExecuteNaiveUnrolled               took 0.14997979 seconds ( 1521107)
        [2019-02-21 17:26:33.961379] [0x00004790] [trace]   CountDisjoint64.ExecuteSSE128                      took 0.10508004 seconds ( 1521107)
        [2019-02-21 17:26:34.066421] [0x00004790] [trace]   CountDisjoint64.ExecuteSSE128WithZero              took 0.10546765 seconds ( 1521107)
        [2019-02-21 17:26:34.169462] [0x00004790] [trace]   CountDisjoint64.ExecuteSSE128Unrolled              took 0.10304863 seconds ( 1521107)
        [2019-02-21 17:26:34.276505] [0x00004790] [trace]   CountDisjoint64.ExecuteSSE128UnrolledStream        took 0.10645892 seconds ( 1521107)
        [2019-02-21 17:26:34.276505] [0x00004790] [trace]   CountDisjoint64.ExecuteNaive                      0.16667616
        [2019-02-21 17:26:34.276505] [0x00004790] [trace]   CountDisjoint64.ExecuteNaiveUnrolled              0.15176798
        [2019-02-21 17:26:34.276505] [0x00004790] [trace]   CountDisjoint64.ExecuteSSE128                     0.10966294
        [2019-02-21 17:26:34.276505] [0x00004790] [trace]   CountDisjoint64.ExecuteSSE128WithZero             0.10813977
        [2019-02-21 17:26:34.276505] [0x00004790] [trace]   CountDisjoint64.ExecuteSSE128UnrolledStream       0.10566424
        [2019-02-21 17:26:34.276505] [0x00004790] [trace]   CountDisjoint64.ExecuteSSE128Unrolled             0.1046457
