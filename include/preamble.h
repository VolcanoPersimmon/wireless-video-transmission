/*! \file preamble.h
 *  \brief Hardcoded preamble samples.
 *
此文件包含 802.11a 前导码样本的时域表示，以及短训练序列和长训练序列样本的频域表示。
 */

#ifndef PREAMBLE_H
#define PREAMBLE_H

#include <complex>

namespace fun
{
    /*! \brief Full 802.11a Preamble in time domain.
     * The preamble consists of 320 samples.  The first 160 samples
     * are made up of 10 copies of the short training sequence which
     * is 16 samples long (16*10 = 160). The second half of the preamble
     * is made up of two long training sequences which are 64 samples each
     * preceded by a 32 sample cyclic prefix which is essentially the last
     * half of one LTS (32+64+64 = 160).
     *
     */
    static std::complex<double> PREAMBLE_SAMPLES[320] =
    {
        std::complex<double>(  0.0229993772561  ,  0.0229993772561  ),
        std::complex<double>( -0.132443716852   ,  0.00233959188499 ),
        std::complex<double>( -0.0134727232705  , -0.0785247857538  ),
        std::complex<double>(  0.142755292821   , -0.0126511678539  ),
        std::complex<double>(  0.0919975090242  ,  0.0              ),
        std::complex<double>(  0.142755292821   , -0.0126511678539  ),
        std::complex<double>( -0.0134727232705  , -0.0785247857538  ),
        std::complex<double>( -0.132443716852   ,  0.00233959188499 ),
        std::complex<double>(  0.0459987545121  ,  0.0459987545121  ),
        std::complex<double>(  0.00233959188499 , -0.132443716852   ),
        std::complex<double>( -0.0785247857538  , -0.0134727232705  ),
        std::complex<double>( -0.0126511678539  ,  0.142755292821   ),
        std::complex<double>(  0.0              ,  0.0919975090242  ),
        std::complex<double>( -0.0126511678539  ,  0.142755292821   ),
        std::complex<double>( -0.0785247857538  , -0.0134727232705  ),
        std::complex<double>(  0.00233959188499 , -0.132443716852   ),

        std::complex<double>(  0.0459987545121  ,  0.0459987545121  ),
        std::complex<double>( -0.132443716852   ,  0.00233959188499 ),
        std::complex<double>( -0.0134727232705  , -0.0785247857538  ),
        std::complex<double>(  0.142755292821   , -0.0126511678539  ),
        std::complex<double>(  0.0919975090242  ,  0.0              ),
        std::complex<double>(  0.142755292821   , -0.0126511678539  ),
        std::complex<double>( -0.0134727232705  , -0.0785247857538  ),
        std::complex<double>( -0.132443716852   ,  0.00233959188499 ),
        std::complex<double>(  0.0459987545121  ,  0.0459987545121  ),
        std::complex<double>(  0.00233959188499 , -0.132443716852   ),
        std::complex<double>( -0.0785247857538  , -0.0134727232705  ),
        std::complex<double>( -0.0126511678539  ,  0.142755292821   ),
        std::complex<double>(  0.0              ,  0.0919975090242  ),
        std::complex<double>( -0.0126511678539  ,  0.142755292821   ),
        std::complex<double>( -0.0785247857538  , -0.0134727232705  ),
        std::complex<double>(  0.00233959188499 , -0.132443716852   ),

        std::complex<double>(  0.0459987545121  ,  0.0459987545121  ),
        std::complex<double>( -0.132443716852   ,  0.00233959188499 ),
        std::complex<double>( -0.0134727232705  , -0.0785247857538  ),
        std::complex<double>(  0.142755292821   , -0.0126511678539  ),
        std::complex<double>(  0.0919975090242  ,  0.0              ),
        std::complex<double>(  0.142755292821   , -0.0126511678539  ),
        std::complex<double>( -0.0134727232705  , -0.0785247857538  ),
        std::complex<double>( -0.132443716852   ,  0.00233959188499 ),
        std::complex<double>(  0.0459987545121  ,  0.0459987545121  ),
        std::complex<double>(  0.00233959188499 , -0.132443716852   ),
        std::complex<double>( -0.0785247857538  , -0.0134727232705  ),
        std::complex<double>( -0.0126511678539  ,  0.142755292821   ),
        std::complex<double>(  0.0              ,  0.0919975090242  ),
        std::complex<double>( -0.0126511678539  ,  0.142755292821   ),
        std::complex<double>( -0.0785247857538  , -0.0134727232705  ),
        std::complex<double>(  0.00233959188499 , -0.132443716852   ),

        std::complex<double>(  0.0459987545121  ,  0.0459987545121  ),
        std::complex<double>( -0.132443716852   ,  0.00233959188499 ),
        std::complex<double>( -0.0134727232705  , -0.0785247857538  ),
        std::complex<double>(  0.142755292821   , -0.0126511678539  ),
        std::complex<double>(  0.0919975090242  ,  0.0              ),
        std::complex<double>(  0.142755292821   , -0.0126511678539  ),
        std::complex<double>( -0.0134727232705  , -0.0785247857538  ),
        std::complex<double>( -0.132443716852   ,  0.00233959188499 ),
        std::complex<double>(  0.0459987545121  ,  0.0459987545121  ),
        std::complex<double>(  0.00233959188499 , -0.132443716852   ),
        std::complex<double>( -0.0785247857538  , -0.0134727232705  ),
        std::complex<double>( -0.0126511678539  ,  0.142755292821   ),
        std::complex<double>(  0.0              ,  0.0919975090242  ),
        std::complex<double>( -0.0126511678539  ,  0.142755292821   ),
        std::complex<double>( -0.0785247857538  , -0.0134727232705  ),
        std::complex<double>(  0.00233959188499 , -0.132443716852   ),

        std::complex<double>(  0.0459987545121  ,  0.0459987545121  ),
        std::complex<double>( -0.132443716852   ,  0.00233959188499 ),
        std::complex<double>( -0.0134727232705  , -0.0785247857538  ),
        std::complex<double>(  0.142755292821   , -0.0126511678539  ),
        std::complex<double>(  0.0919975090242  ,  0.0              ),
        std::complex<double>(  0.142755292821   , -0.0126511678539  ),
        std::complex<double>( -0.0134727232705  , -0.0785247857538  ),
        std::complex<double>( -0.132443716852   ,  0.00233959188499 ),
        std::complex<double>(  0.0459987545121  ,  0.0459987545121  ),
        std::complex<double>(  0.00233959188499 , -0.132443716852   ),
        std::complex<double>( -0.0785247857538  , -0.0134727232705  ),
        std::complex<double>( -0.0126511678539  ,  0.142755292821   ),
        std::complex<double>(  0.0              ,  0.0919975090242  ),
        std::complex<double>( -0.0126511678539  ,  0.142755292821   ),
        std::complex<double>( -0.0785247857538  , -0.0134727232705  ),
        std::complex<double>(  0.00233959188499 , -0.132443716852   ),

        std::complex<double>(  0.0459987545121  ,  0.0459987545121  ),
        std::complex<double>( -0.132443716852   ,  0.00233959188499 ),
        std::complex<double>( -0.0134727232705  , -0.0785247857538  ),
        std::complex<double>(  0.142755292821   , -0.0126511678539  ),
        std::complex<double>(  0.0919975090242  ,  0.0              ),
        std::complex<double>(  0.142755292821   , -0.0126511678539  ),
        std::complex<double>( -0.0134727232705  , -0.0785247857538  ),
        std::complex<double>( -0.132443716852   ,  0.00233959188499 ),
        std::complex<double>(  0.0459987545121  ,  0.0459987545121  ),
        std::complex<double>(  0.00233959188499 , -0.132443716852   ),
        std::complex<double>( -0.0785247857538  , -0.0134727232705  ),
        std::complex<double>( -0.0126511678539  ,  0.142755292821   ),
        std::complex<double>(  0.0              ,  0.0919975090242  ),
        std::complex<double>( -0.0126511678539  ,  0.142755292821   ),
        std::complex<double>( -0.0785247857538  , -0.0134727232705  ),
        std::complex<double>(  0.00233959188499 , -0.132443716852   ),

        std::complex<double>(  0.0459987545121  ,  0.0459987545121  ),
        std::complex<double>( -0.132443716852   ,  0.00233959188499 ),
        std::complex<double>( -0.0134727232705  , -0.0785247857538  ),
        std::complex<double>(  0.142755292821   , -0.0126511678539  ),
        std::complex<double>(  0.0919975090242  ,  0.0              ),
        std::complex<double>(  0.142755292821   , -0.0126511678539  ),
        std::complex<double>( -0.0134727232705  , -0.0785247857538  ),
        std::complex<double>( -0.132443716852   ,  0.00233959188499 ),
        std::complex<double>(  0.0459987545121  ,  0.0459987545121  ),
        std::complex<double>(  0.00233959188499 , -0.132443716852   ),
        std::complex<double>( -0.0785247857538  , -0.0134727232705  ),
        std::complex<double>( -0.0126511678539  ,  0.142755292821   ),
        std::complex<double>(  0.0              ,  0.0919975090242  ),
        std::complex<double>( -0.0126511678539  ,  0.142755292821   ),
        std::complex<double>( -0.0785247857538  , -0.0134727232705  ),
        std::complex<double>(  0.00233959188499 , -0.132443716852   ),

        std::complex<double>(  0.0459987545121  ,  0.0459987545121  ),
        std::complex<double>( -0.132443716852   ,  0.00233959188499 ),
        std::complex<double>( -0.0134727232705  , -0.0785247857538  ),
        std::complex<double>(  0.142755292821   , -0.0126511678539  ),
        std::complex<double>(  0.0919975090242  ,  0.0              ),
        std::complex<double>(  0.142755292821   , -0.0126511678539  ),
        std::complex<double>( -0.0134727232705  , -0.0785247857538  ),
        std::complex<double>( -0.132443716852   ,  0.00233959188499 ),
        std::complex<double>(  0.0459987545121  ,  0.0459987545121  ),
        std::complex<double>(  0.00233959188499 , -0.132443716852   ),
        std::complex<double>( -0.0785247857538  , -0.0134727232705  ),
        std::complex<double>( -0.0126511678539  ,  0.142755292821   ),
        std::complex<double>(  0.0              ,  0.0919975090242  ),
        std::complex<double>( -0.0126511678539  ,  0.142755292821   ),
        std::complex<double>( -0.0785247857538  , -0.0134727232705  ),
        std::complex<double>(  0.00233959188499 , -0.132443716852   ),

        std::complex<double>(  0.0459987545121  ,  0.0459987545121  ),
        std::complex<double>( -0.132443716852   ,  0.00233959188499 ),
        std::complex<double>( -0.0134727232705  , -0.0785247857538  ),
        std::complex<double>(  0.142755292821   , -0.0126511678539  ),
        std::complex<double>(  0.0919975090242  ,  0.0              ),
        std::complex<double>(  0.142755292821   , -0.0126511678539  ),
        std::complex<double>( -0.0134727232705  , -0.0785247857538  ),
        std::complex<double>( -0.132443716852   ,  0.00233959188499 ),
        std::complex<double>(  0.0459987545121  ,  0.0459987545121  ),
        std::complex<double>(  0.00233959188499 , -0.132443716852   ),
        std::complex<double>( -0.0785247857538  , -0.0134727232705  ),
        std::complex<double>( -0.0126511678539  ,  0.142755292821   ),
        std::complex<double>(  0.0              ,  0.0919975090242  ),
        std::complex<double>( -0.0126511678539  ,  0.142755292821   ),
        std::complex<double>( -0.0785247857538  , -0.0134727232705  ),
        std::complex<double>(  0.00233959188499 , -0.132443716852   ),

        std::complex<double>(  0.0459987545121  ,  0.0459987545121  ),
        std::complex<double>( -0.132443716852   ,  0.00233959188499 ),
        std::complex<double>( -0.0134727232705  , -0.0785247857538  ),
        std::complex<double>(  0.142755292821   , -0.0126511678539  ),
        std::complex<double>(  0.0919975090242  ,  0.0              ),
        std::complex<double>(  0.142755292821   , -0.0126511678539  ),
        std::complex<double>( -0.0134727232705  , -0.0785247857538  ),
        std::complex<double>( -0.132443716852   ,  0.00233959188499 ),
        std::complex<double>(  0.0459987545121  ,  0.0459987545121  ),
        std::complex<double>(  0.00233959188499 , -0.132443716852   ),
        std::complex<double>( -0.0785247857538  , -0.0134727232705  ),
        std::complex<double>( -0.0126511678539  ,  0.142755292821   ),
        std::complex<double>(  0.0              ,  0.0919975090242  ),
        std::complex<double>( -0.0126511678539  ,  0.142755292821   ),
        std::complex<double>( -0.0785247857538  , -0.0134727232705  ),
        std::complex<double>(  0.00233959188499 , -0.132443716852   ),

        //Long Training seque nce
        std::complex<double>( -0.078            ,  0.0),
        std::complex<double>(  0.0122845904586  , -0.0975995535921  ),
        std::complex<double>(  0.0917165491224  , -0.105871659819   ),
        std::complex<double>( -0.0918875552628  , -0.115128708911   ),
        std::complex<double>( -0.00280594417349 , -0.0537742664765  ),
        std::complex<double>(  0.0750736970682  ,  0.0740404189251  ),
        std::complex<double>( -0.127324359908   ,  0.0205013799863  ),
        std::complex<double>( -0.121887009061   ,  0.0165662181391  ),
        std::complex<double>( -0.0350412607362  ,  0.150888347648   ),
        std::complex<double>( -0.0564551284485  ,  0.0218039206074  ),
        std::complex<double>( -0.0603101003162  , -0.0812861241157  ),
        std::complex<double>(  0.0695568474069  , -0.0141219585906  ),
        std::complex<double>(  0.0822183223031  , -0.0923565519537  ),
        std::complex<double>( -0.131262608975   , -0.0652272290181  ),
        std::complex<double>( -0.0572063458715  , -0.0392985881741  ),
        std::complex<double>(  0.0369179420011  , -0.0983441502871  ),
        std::complex<double>(  0.0625           ,  0.0625           ),
        std::complex<double>(  0.11923908851    ,  0.0040955944148  ),
        std::complex<double>( -0.0224832063078  , -0.160657332953   ),
        std::complex<double>(  0.0586687671287  ,  0.0149389994507  ),
        std::complex<double>(  0.0244758515211  ,  0.0585317956946  ),
        std::complex<double>( -0.136804876816   ,  0.0473798113657  ),
        std::complex<double>(  0.000988979708988,  0.115004643624   ),
        std::complex<double>(  0.0533377343742  , -0.00407632648051 ),
        std::complex<double>(  0.0975412607362  ,  0.0258883476483  ),
        std::complex<double>( -0.0383159674744  ,  0.106170912615   ),
        std::complex<double>( -0.115131214782   ,  0.0551804953744  ),
        std::complex<double>(  0.059823844859   ,  0.0877067598357  ),
        std::complex<double>(  0.0211117703493  , -0.0278859188282  ),
        std::complex<double>(  0.0968318845911  , -0.0827979094878  ),
        std::complex<double>(  0.0397496983535  ,  0.111157943051   ),
        std::complex<double>( -0.00512125036042 ,  0.120325132674   ),

        std::complex<double>(  0.15625          ,  0.0              ),
        std::complex<double>( -0.00512125036042 , -0.120325132674   ),
        std::complex<double>(  0.0397496983535  , -0.111157943051   ),
        std::complex<double>(  0.0968318845911  ,  0.0827979094878  ),
        std::complex<double>(  0.0211117703493  ,  0.0278859188282  ),
        std::complex<double>(  0.059823844859   , -0.0877067598357  ),
        std::complex<double>( -0.115131214782   , -0.0551804953744  ),
        std::complex<double>( -0.0383159674744  , -0.106170912615   ),
        std::complex<double>(  0.0975412607362  , -0.0258883476483  ),
        std::complex<double>(  0.0533377343742  ,  0.00407632648051 ),
        std::complex<double>(  0.000988979708988, -0.115004643624   ),
        std::complex<double>( -0.136804876816   , -0.0473798113657  ),
        std::complex<double>(  0.0244758515211  , -0.0585317956946  ),
        std::complex<double>(  0.0586687671287  , -0.0149389994507  ),
        std::complex<double>( -0.0224832063078  ,  0.160657332953   ),
        std::complex<double>(  0.11923908851    , -0.0040955944148  ),
        std::complex<double>(  0.0625           , -0.0625           ),
        std::complex<double>(  0.0369179420011  ,  0.0983441502871  ),
        std::complex<double>( -0.0572063458715  ,  0.0392985881741  ),
        std::complex<double>( -0.131262608975   ,  0.0652272290181  ),
        std::complex<double>(  0.0822183223031  ,  0.0923565519537  ),
        std::complex<double>(  0.0695568474069  ,  0.0141219585906  ),
        std::complex<double>( -0.0603101003162  ,  0.0812861241157  ),
        std::complex<double>( -0.0564551284485  , -0.0218039206074  ),
        std::complex<double>( -0.0350412607362  , -0.150888347648   ) ,
        std::complex<double>( -0.121887009061   , -0.0165662181391  ),
        std::complex<double>( -0.127324359908   , -0.0205013799863  ),
        std::complex<double>(  0.0750736970682  , -0.0740404189251  ),
        std::complex<double>( -0.00280594417349 ,  0.0537742664765  ),
        std::complex<double>( -0.0918875552628  ,  0.115128708911   ),
        std::complex<double>(  0.0917165491224  ,  0.105871659819   ),
        std::complex<double>(  0.0122845904586  ,  0.0975995535921  ),
        std::complex<double>( -0.15625          ,  0.0              ),
        std::complex<double>(  0.0122845904586  , -0.0975995535921  ),
        std::complex<double>(  0.0917165491224  , -0.105871659819   ),
        std::complex<double>( -0.0918875552628  , -0.115128708911   ),
        std::complex<double>( -0.00280594417349 , -0.0537742664765  ),
        std::complex<double>(  0.0750736970682  ,  0.0740404189251  ),
        std::complex<double>( -0.127324359908   ,  0.0205013799863  ),
        std::complex<double>( -0.121887009061   ,  0.0165662181391  ),
        std::complex<double>( -0.0350412607362  ,  0.150888347648   ),
        std::complex<double>( -0.0564551284485  ,  0.0218039206074  ),
        std::complex<double>( -0.0603101003162  , -0.0812861241157  ),
        std::complex<double>(  0.0695568474069  , -0.0141219585906  ),
        std::complex<double>(  0.0822183223031  , -0.0923565519537  ),
        std::complex<double>( -0.131262608975   , -0.0652272290181  ),
        std::complex<double>( -0.0572063458715  , -0.0392985881741  ),
        std::complex<double>(  0.0369179420011  , -0.0983441502871  ),
        std::complex<double>(  0.0625           ,  0.0625           ),
        std::complex<double>(  0.11923908851    ,  0.0040955944148  ),
        std::complex<double>( -0.0224832063078  , -0.160657332953   ),
        std::complex<double>(  0.0586687671287  ,  0.0149389994507  ),
        std::complex<double>(  0.0244758515211  ,  0.0585317956946  ),
        std::complex<double>( -0.136804876816   ,  0.0473798113657  ),
        std::complex<double>(  0.000988979708988,  0.115004643624   ),
        std::complex<double>(  0.0533377343742  , -0.00407632648051 ),
        std::complex<double>(  0.0975412607362  ,  0.0258883476483  ),
        std::complex<double>( -0.0383159674744  ,  0.106170912615   ),
        std::complex<double>( -0.115131214782   ,  0.0551804953744  ),
        std::complex<double>(  0.059823844859   ,  0.0877067598357  ),
        std::complex<double>(  0.0211117703493  , -0.0278859188282  ),
        std::complex<double>(  0.0968318845911  , -0.0827979094878  ),
        std::complex<double>(  0.0397496983535  ,  0.111157943051   ),
        std::complex<double>( -0.00512125036042 ,  0.120325132674   ),

        std::complex<double>(  0.15625          ,  0.0              ),
        std::complex<double>( -0.00512125036042 , -0.120325132674   ),
        std::complex<double>(  0.0397496983535  , -0.111157943051   ),
        std::complex<double>(  0.0968318845911  ,  0.0827979094878  ),
        std::complex<double>(  0.0211117703493  ,  0.0278859188282  ),
        std::complex<double>(  0.059823844859   , -0.0877067598357  ),
        std::complex<double>( -0.115131214782   , -0.0551804953744  ),
        std::complex<double>( -0.0383159674744  , -0.106170912615   ),
        std::complex<double>(  0.0975412607362  , -0.0258883476483  ),
        std::complex<double>(  0.0533377343742  ,  0.00407632648051 ),
        std::complex<double>(  0.000988979708988, -0.115004643624   ),
        std::complex<double>( -0.136804876816   , -0.0473798113657  ),
        std::complex<double>(  0.0244758515211  , -0.0585317956946  ),
        std::complex<double>(  0.0586687671287  , -0.0149389994507  ),
        std::complex<double>( -0.0224832063078  ,  0.160657332953   ),
        std::complex<double>(  0.11923908851    , -0.0040955944148  ),
        std::complex<double>(  0.0625           , -0.0625           ),
        std::complex<double>(  0.0369179420011  ,  0.0983441502871  ),
        std::complex<double>( -0.0572063458715  ,  0.0392985881741  ),
        std::complex<double>( -0.131262608975   ,  0.0652272290181  ),
        std::complex<double>(  0.0822183223031  ,  0.0923565519537  ),
        std::complex<double>(  0.0695568474069  ,  0.0141219585906  ),
        std::complex<double>( -0.0603101003162  ,  0.0812861241157  ),
        std::complex<double>( -0.0564551284485  , -0.0218039206074  ),
        std::complex<double>( -0.0350412607362  , -0.150888347648   ),
        std::complex<double>( -0.121887009061   , -0.0165662181391  ),
        std::complex<double>( -0.127324359908   , -0.0205013799863  ),
        std::complex<double>(  0.0750736970682  , -0.0740404189251  ),
        std::complex<double>( -0.00280594417349 ,  0.0537742664765  ),
        std::complex<double>( -0.0918875552628  ,  0.115128708911   ),
        std::complex<double>(  0.0917165491224  ,  0.105871659819   ),
        std::complex<double>(  0.0122845904586  ,  0.0975995535921  ),
        std::complex<double>( -0.15625          ,  0.0              ),
        std::complex<double>(  0.0122845904586  , -0.0975995535921  ),
        std::complex<double>(  0.0917165491224  , -0.105871659819   ),
        std::complex<double>( -0.0918875552628  , -0.115128708911   ),
        std::complex<double>( -0.00280594417349 , -0.0537742664765  ),
        std::complex<double>(  0.0750736970682  ,  0.0740404189251  ),
        std::complex<double>( -0.127324359908   ,  0.0205013799863  ),
        std::complex<double>( -0.121887009061   ,  0.0165662181391  ),
        std::complex<double>( -0.0350412607362  ,  0.150888347648   ),
        std::complex<double>( -0.0564551284485  ,  0.0218039206074  ),
        std::complex<double>( -0.0603101003162  , -0.0812861241157  ),
        std::complex<double>(  0.0695568474069  , -0.0141219585906  ),
        std::complex<double>(  0.0822183223031  , -0.0923565519537  ),
        std::complex<double>( -0.131262608975   , -0.0652272290181  ),
        std::complex<double>( -0.0572063458715  , -0.0392985881741  ),
        std::complex<double>(  0.0369179420011  , -0.0983441502871  ),
        std::complex<double>(  0.0625           ,  0.0625           ),
        std::complex<double>(  0.11923908851    ,  0.0040955944148  ),
        std::complex<double>( -0.0224832063078  , -0.160657332953   ),
        std::complex<double>(  0.0586687671287  ,  0.0149389994507  ),
        std::complex<double>(  0.0244758515211  ,  0.0585317956946  ),
        std::complex<double>( -0.136804876816   ,  0.0473798113657  ),
        std::complex<double>(  0.000988979708988,  0.115004643624   ),
        std::complex<double>(  0.0533377343742  , -0.00407632648051 ),
        std::complex<double>(  0.0975412607362  ,  0.0258883476483  ),
        std::complex<double>( -0.0383159674744  ,  0.106170912615   ),
        std::complex<double>( -0.115131214782   ,  0.0551804953744  ),
        std::complex<double>(  0.059823844859   ,  0.0877067598357  ),
        std::complex<double>(  0.0211117703493  , -0.0278859188282  ),
        std::complex<double>(  0.0968318845911  , -0.0827979094878  ),
        std::complex<double>(  0.0397496983535  ,  0.111157943051   ),
        std::complex<double>( -0.00512125036042 ,  0.120325132674   )
    };


    /*! \brief Long Training Sequence in frequency domain. */
    static std::complex<double> LTS_FREQ_DOMAIN[64] =
    {
        std::complex<double>( 0,  0 ),
        std::complex<double>( 0,  0 ),
        std::complex<double>( 0,  0 ),
        std::complex<double>( 0,  0 ),
        std::complex<double>( 0,  0 ),
        std::complex<double>( 0,  0 ),
        std::complex<double>( 1,  0 ),
        std::complex<double>( 1,  0 ),
        std::complex<double>(-1,  0 ),
        std::complex<double>(-1,  0 ),
        std::complex<double>( 1,  0 ),
        std::complex<double>( 1,  0 ),
        std::complex<double>(-1,  0 ),
        std::complex<double>( 1,  0 ),
        std::complex<double>(-1,  0 ),
        std::complex<double>( 1,  0 ),
        std::complex<double>( 1,  0 ),
        std::complex<double>( 1,  0 ),
        std::complex<double>( 1,  0 ),
        std::complex<double>( 1,  0 ),
        std::complex<double>( 1,  0 ),
        std::complex<double>(-1,  0 ),
        std::complex<double>(-1,  0 ),
        std::complex<double>( 1,  0 ),
        std::complex<double>( 1,  0 ),
        std::complex<double>(-1,  0 ),
        std::complex<double>( 1,  0 ),
        std::complex<double>(-1,  0 ),
        std::complex<double>( 1,  0 ),
        std::complex<double>( 1,  0 ),
        std::complex<double>( 1,  0 ),
        std::complex<double>( 1,  0 ),
        std::complex<double>( 0,  0 ),
        std::complex<double>( 1,  0 ),
        std::complex<double>(-1,  0 ),
        std::complex<double>(-1,  0 ),
        std::complex<double>( 1,  0 ),
        std::complex<double>( 1,  0 ),
        std::complex<double>(-1,  0 ),
        std::complex<double>( 1,  0 ),
        std::complex<double>(-1,  0 ),
        std::complex<double>( 1,  0 ),
        std::complex<double>(-1,  0 ),
        std::complex<double>(-1,  0 ),
        std::complex<double>(-1,  0 ),
        std::complex<double>(-1,  0 ),
        std::complex<double>(-1,  0 ),
        std::complex<double>( 1,  0 ),
        std::complex<double>( 1,  0 ),
        std::complex<double>(-1,  0 ),
        std::complex<double>(-1,  0 ),
        std::complex<double>( 1,  0 ),
        std::complex<double>(-1,  0 ),
        std::complex<double>( 1,  0 ),
        std::complex<double>(-1,  0 ),
        std::complex<double>( 1,  0 ),
        std::complex<double>( 1,  0 ),
        std::complex<double>( 1,  0 ),
        std::complex<double>( 1,  0 ),
        std::complex<double>( 0,  0 ),
        std::complex<double>( 0,  0 ),
        std::complex<double>( 0,  0 ),
        std::complex<double>( 0,  0 ),
        std::complex<double>( 0,  0 )
    };

    /*! \brief Complex conjugate of Long Training Sequence in time domain. */
    static std::complex<double> LTS_TIME_DOMAIN_CONJ[64] =
    {
        std::complex<double>( 0.15625          ,  0.0),
        std::complex<double>(-0.00512125036042 ,  0.120325132674),
        std::complex<double>( 0.0397496983535  ,  0.111157943051),
        std::complex<double>( 0.0968318845911  , -0.0827979094878),
        std::complex<double>( 0.0211117703493  , -0.0278859188282),
        std::complex<double>( 0.059823844859   ,  0.0877067598357),
        std::complex<double>(-0.115131214782   ,  0.0551804953744),
        std::complex<double>(-0.0383159674744  ,  0.106170912615),
        std::complex<double>( 0.0975412607362  ,  0.0258883476483),
        std::complex<double>( 0.0533377343742  , -0.00407632648051),
        std::complex<double>( 0.000988979708988,  0.115004643624),
        std::complex<double>(-0.136804876816   ,  0.0473798113657),
        std::complex<double>( 0.0244758515211  ,  0.0585317956946),
        std::complex<double>( 0.0586687671287  ,  0.0149389994507),
        std::complex<double>(-0.0224832063078  , -0.160657332953),
        std::complex<double>( 0.11923908851    ,  0.0040955944148),
        std::complex<double>( 0.0625           ,  0.0625),
        std::complex<double>( 0.0369179420011  , -0.0983441502871),
        std::complex<double>(-0.0572063458715  , -0.0392985881741),
        std::complex<double>(-0.131262608975   , -0.0652272290181),
        std::complex<double>( 0.0822183223031  , -0.0923565519537),
        std::complex<double>( 0.0695568474069  , -0.0141219585906),
        std::complex<double>(-0.0603101003162  , -0.0812861241157),
        std::complex<double>(-0.0564551284485  ,  0.0218039206074),
        std::complex<double>(-0.0350412607362  ,  0.150888347648),
        std::complex<double>(-0.121887009061   ,  0.0165662181391),
        std::complex<double>(-0.127324359908   ,  0.0205013799863),
        std::complex<double>( 0.0750736970682  ,  0.0740404189251),
        std::complex<double>(-0.00280594417349 , -0.0537742664765),
        std::complex<double>(-0.0918875552628  , -0.115128708911),
        std::complex<double>( 0.0917165491224  , -0.105871659819),
        std::complex<double>( 0.0122845904586  , -0.0975995535921),
        std::complex<double>(-0.15625          , -0.0),
        std::complex<double>( 0.0122845904586  ,  0.0975995535921),
        std::complex<double>( 0.0917165491224  ,  0.105871659819),
        std::complex<double>(-0.0918875552628  ,  0.115128708911),
        std::complex<double>(-0.00280594417349 ,  0.0537742664765),
        std::complex<double>( 0.0750736970682  , -0.0740404189251),
        std::complex<double>(-0.127324359908   , -0.0205013799863),
        std::complex<double>(-0.121887009061   , -0.0165662181391),
        std::complex<double>(-0.0350412607362  , -0.150888347648),
        std::complex<double>(-0.0564551284485  , -0.0218039206074),
        std::complex<double>(-0.0603101003162  ,  0.0812861241157),
        std::complex<double>( 0.0695568474069  ,  0.0141219585906),
        std::complex<double>( 0.0822183223031  ,  0.0923565519537),
        std::complex<double>(-0.131262608975   ,  0.0652272290181),
        std::complex<double>(-0.0572063458715  ,  0.0392985881741),
        std::complex<double>( 0.0369179420011  ,  0.0983441502871),
        std::complex<double>( 0.0625           , -0.0625),
        std::complex<double>( 0.11923908851    , -0.0040955944148),
        std::complex<double>(-0.0224832063078  ,  0.160657332953),
        std::complex<double>( 0.0586687671287  , -0.0149389994507),
        std::complex<double>( 0.0244758515211  , -0.0585317956946),
        std::complex<double>(-0.136804876816   , -0.0473798113657),
        std::complex<double>( 0.000988979708988, -0.115004643624),
        std::complex<double>( 0.0533377343742  ,  0.00407632648051),
        std::complex<double>( 0.0975412607362  , -0.0258883476483),
        std::complex<double>(-0.0383159674744  , -0.106170912615),
        std::complex<double>(-0.115131214782   , -0.0551804953744),
        std::complex<double>( 0.059823844859   , -0.0877067598357),
        std::complex<double>( 0.0211117703493  ,  0.0278859188282),
        std::complex<double>( 0.0968318845911  ,  0.0827979094878),
        std::complex<double>( 0.0397496983535  , -0.111157943051),
        std::complex<double>(-0.00512125036042 , -0.120325132674)
    };

    /*! \brief Short Training Sequence in time domain. */
    static std::complex<double> STS_SAMPLES[16] =
    {
        std::complex<double>( 0.0459987545121 ,  0.0459987545121),
        std::complex<double>(-0.132443716852  ,  0.00233959188499),
        std::complex<double>(-0.0134727232705 , -0.0785247857538),
        std::complex<double>( 0.142755292821  , -0.0126511678539),
        std::complex<double>( 0.0919975090242 ,  0.0),
        std::complex<double>( 0.142755292821  , -0.0126511678539),
        std::complex<double>(-0.0134727232705 , -0.0785247857538),
        std::complex<double>(-0.132443716852  ,  0.00233959188499),
        std::complex<double>( 0.0459987545121 ,  0.0459987545121),
        std::complex<double>( 0.00233959188499, -0.132443716852),
        std::complex<double>(-0.0785247857538 , -0.0134727232705),
        std::complex<double>(-0.0126511678539 ,  0.142755292821),
        std::complex<double>( 0.0             ,  0.0919975090242),
        std::complex<double>(-0.0126511678539 ,  0.142755292821),
        std::complex<double>(-0.0785247857538 , -0.0134727232705),
        std::complex<double>( 0.00233959188499, -0.132443716852),
    };

}

#endif // PREAMBLE_H
