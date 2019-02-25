personal code trying to figure out optimial SSE execution.
I started this because I wanted to find the quickest way to count the number of disjoint bitmask in a vector
        
        ./driver # Linux 
        [2019-02-25 20:18:12.704241] [0x00007f82389147c0] [trace]   MaxElement.ExecuteStride                          0.038813682 seconds
        [2019-02-25 20:18:12.704252] [0x00007f82389147c0] [trace]   MaxElement.ExecuteStl                             0.034253724 seconds
        [2019-02-25 20:18:12.704254] [0x00007f82389147c0] [trace]   MaxElement.ExecuteBoostRange                      0.034121781 seconds
        [2019-02-25 20:18:12.704256] [0x00007f82389147c0] [trace]   MaxElement.ExecuteStlRaw                          0.034098375 seconds
        [2019-02-25 20:18:12.704257] [0x00007f82389147c0] [trace]   MaxElement.ExecuteSSE                             0.027576269 seconds
        [2019-02-25 20:18:12.704259] [0x00007f82389147c0] [trace]   MaxElement.ExecuteStrideRaw                       0.020964617 seconds
        [2019-02-25 20:18:12.704260] [0x00007f82389147c0] [trace]   MaxElement.ExecuteSSELoad                         0.019754865 seconds
        [2019-02-25 20:18:12.704262] [0x00007f82389147c0] [trace]   MaxElement.ExecuteSSELoadOneReg                   0.019739701 seconds
        [2019-02-25 20:18:12.704263] [0x00007f82389147c0] [trace]   MaxElement.ExecuteSSEStide                        0.019666252 seconds
        [2019-02-25 20:18:12.704265] [0x00007f82389147c0] [trace]   MaxElement.ExecuteNaiveRaw                        0.018623928 seconds
        [2019-02-25 20:18:12.704266] [0x00007f82389147c0] [trace]   MaxElement.ExecuteNaive                           0.018622805 seconds
        [2019-02-25 20:18:14.091870] [0x00007f82389147c0] [trace]   CountDisjoint.ExecuteNaiveUnrolled                0.048532231 seconds
        [2019-02-25 20:18:14.091875] [0x00007f82389147c0] [trace]   CountDisjoint.ExecuteSSEPopcount                  0.026181638 seconds
        [2019-02-25 20:18:14.091877] [0x00007f82389147c0] [trace]   CountDisjoint.ExecuteSSEStream                    0.022496265 seconds
        [2019-02-25 20:18:14.091879] [0x00007f82389147c0] [trace]   CountDisjoint.ExecuteSSE                          0.02192771 seconds
        [2019-02-25 20:18:14.091880] [0x00007f82389147c0] [trace]   CountDisjoint.ExecuteNaive                        0.019603402 seconds
        [2019-02-25 20:18:17.040011] [0x00007f82389147c0] [trace]   CountDisjoint64.ExecuteSSE128WithZero             0.043961766 seconds
        [2019-02-25 20:18:17.040016] [0x00007f82389147c0] [trace]   CountDisjoint64.ExecuteSSE128                     0.0439052 seconds
        [2019-02-25 20:18:17.040018] [0x00007f82389147c0] [trace]   CountDisjoint64.ExecuteSSE128UnrolledStream       0.043235714 seconds
        [2019-02-25 20:18:17.040020] [0x00007f82389147c0] [trace]   CountDisjoint64.ExecuteSSE128Unrolled             0.043077846 seconds
        [2019-02-25 20:18:17.040021] [0x00007f82389147c0] [trace]   CountDisjoint64.ExecuteNaiveUnrolled              0.041908317 seconds
        [2019-02-25 20:18:17.040023] [0x00007f82389147c0] [trace]   CountDisjoint64.ExecuteSSE256                     0.039458407 seconds
        [2019-02-25 20:18:17.040025] [0x00007f82389147c0] [trace]   CountDisjoint64.ExecuteNaive                      0.039236047 seconds

        ./driver # Windows
        [2019-02-21 17:26:30.540011] [0x00004790] [trace]   CountDisjoint.ExecuteNaive                        0.098498584
        [2019-02-21 17:26:30.540011] [0x00004790] [trace]   CountDisjoint.ExecuteNaiveUnrolled                0.069359377
        [2019-02-21 17:26:30.540011] [0x00004790] [trace]   CountDisjoint.ExecuteSSEPopcount                  0.054075212
        [2019-02-21 17:26:30.540011] [0x00004790] [trace]   CountDisjoint.ExecuteSSE                          0.048847897
        [2019-02-21 17:26:30.540011] [0x00004790] [trace]   CountDisjoint.ExecuteSSEStream                    0.045773996
        [2019-02-21 17:26:34.276505] [0x00004790] [trace]   CountDisjoint64.ExecuteNaive                      0.16667616
        [2019-02-21 17:26:34.276505] [0x00004790] [trace]   CountDisjoint64.ExecuteNaiveUnrolled              0.15176798
        [2019-02-21 17:26:34.276505] [0x00004790] [trace]   CountDisjoint64.ExecuteSSE128                     0.10966294
        [2019-02-21 17:26:34.276505] [0x00004790] [trace]   CountDisjoint64.ExecuteSSE128WithZero             0.10813977
        [2019-02-21 17:26:34.276505] [0x00004790] [trace]   CountDisjoint64.ExecuteSSE128UnrolledStream       0.10566424
        [2019-02-21 17:26:34.276505] [0x00004790] [trace]   CountDisjoint64.ExecuteSSE128Unrolled             0.1046457
