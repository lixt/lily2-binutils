

.file "t_cover.s"

.text
.align 4

.func t_cover_a
.type t_cover_a,@function

t_cover_a:

    # OP_HI = 000
    [xa] { cf0} add x0,x1,x2
    [ya] {!cf0} add y0,y1,y2
    [xa]        add x0,g0,g2
    [ya]        add g0,g1,g2

    [xa] { cf1} adc x0,x1,x2
    [ya] {!cf1} adc y0,y1,y2
    [xa]        adc x0,g0,g2
    [ya]        adc g0,g1,g2

    [xa] { cf2} sub x0,x1,x2
    [ya] {!cf2} sub y0,y1,y2
    [xa]        sub x0,g0,g2
    [ya]        sub g0,g1,g2

    [xa] { cf0} sbc x0,x1,x2
    [ya] { cf1} sbc y0,y1,y2
    [xa] { cf2} sbc x0,g0,g2
    [ya]        sbc g0,g1,g2

    [xa] {!cf0} rsb x0,x1,x2
    [ya] {!cf1} rsb y0,y1,y2
    [xa] {!cf2} rsb x0,g0,g2
    [ya]        rsb g0,g1,g2

    [xa]        rsc x0,x1,x2
    [ya]        rsc y0,y1,y2
    [xa]        rsc x0,g0,g2
    [ya]        rsc g0,g1,g2

    [xa] {!cf0} abs x20,x21
    [ya] { cf0} abs y20,y21

    [xa] {!cf1} neg x22,x23
    [ya] { cf1} neg y22,y23

    [xa] {!cf2} zxb x24,g7
    [ya] { cf2} zxb y24,g6

    [xa]        zxh y24,g5
    [ya]        zxh x24,g4

    [xa]        sxb x31,x31
    [ya]        sxb y31,y31

    [xa]        sxh y31,y31
    [ya]        sxh x31,x31

    # I: 8-bit signed immediate(-128 to 127).
    [xa] { cf0} add.i x0,x1,-128
    [ya] {!cf0} add.i y0,y1,127
    [xa]        add.i x0,g0,0x11
    [ya]        add.i g0,g1,0x1

    [xa] { cf1} adc.i x0,x1,-127
    [ya] {!cf1} adc.i y0,y1,126
    [xa]        adc.i x0,g0,0
    [ya]        adc.i g0,g1,0xff

    [xa] { cf2} sub.i x0,x1,0x0ff
    [ya] {!cf2} sub.i y0,y1,0x00ff
    [xa]        sub.i x0,g0,0x000ff
    [ya]        sub.i g0,g1,0x0000f1

    [xa] { cf0} sbc.i x0,x1,-1
    [ya] { cf1} sbc.i y0,y1,-2
    [xa] { cf2} sbc.i x0,g0,1
    [ya]        sbc.i g0,g1,2

    [xa] {!cf0} rsb.i x0,x1,0xf0
    [ya] {!cf1} rsb.i y0,y1,0xf1
    [xa] {!cf2} rsb.i x0,g0,0x0f0
    [ya]        rsb.i g0,g1,0x80

    [xa]        rsc.i x0,x1,0x10
    [ya]        rsc.i y0,y1,0x00
    [xa]        rsc.i x0,g0,0xee
    [ya]        rsc.i g0,g1,0xEF



.size t_cover_a,.-t_cover_a
.endfunc

.func t_cover_m
.type t_cover_m, @function

t_cover_m:

#[xx]        brr x0

.size t_cover_m,.-t_cover_m
.endfunc




.func t_cover_d
.type t_cover_d, @function

t_cover_d:

    # OP_HI = 0b'000
    [xd] { cf0} ldb x0,x1(x2)
    [yd] {!cf0} ldb y0,y1(y2)

    [xd] { cf1} ldh x3,x4(x5)
    [yd] {!cf1} ldh y3,y4(y5)

    [xd] { cf2} ldw x6,x7(x8)
    [yd] {!cf2} ldw y6,y7(y8)

    [xd]        ldd x9:x8,x10(x11)
    [yd]        ldd y9:y8,y10(y11)

    [xd] { cf0} stb x0(x1),x2
    [yd] {!cf0} stb y0(y1),y2

    [xd] { cf1} sth x3(x4),x5
    [yd] {!cf1} sth y3(y4),y5

    [xd] { cf2} stw x6(x7),x8
    [yd] {!cf2} stw y6(y7),y8

    [xd]        std x9(x10),x11:x10
    [yd]        std y9(y10),y11:y10

    [xd] { cf0} ldb.u x12,x13(x14)
    [yd] {!cf0} ldb.u y12,y13(y14)

    [xd] { cf1} ldh.u x15,x16(x17)
    [yd] {!cf1} ldh.u y15,y16(y17)

    # I: 8-bit signed immediate(-128 to 127).
    [xd] { cf0} ldb.i x0,127(x2)
    [yd] {!cf0} ldb.i y0,0xff(y2)

    [xd] { cf1} ldh.i x3,-128(x5)
    [yd] {!cf1} ldh.i y3,0(y5)

    [xd] { cf2} ldw.i x6,2(x8)
    [yd] {!cf2} ldw.i y6,20(y8)

    [xd]        ldd.i x11:x10,-99(x11)
    [yd]        ldd.i y11:y10,0x1f(y11)

    [xd] { cf0} stb.i 0xfe(x1),x2
    [yd] {!cf0} stb.i 0x80(y1),y2

    [xd] { cf1} sth.i 126(x4),x5
    [yd] {!cf1} sth.i -127(y4),y5

    [xd] { cf2} stw.i 20(x7),x8
    [yd] {!cf2} stw.i 01(y7),y8

    [xd]        std.i 111(x10),x11:x10
    [yd]        std.i 0x01(y10),y11:y10

    [xd] { cf0} ldb.u.i x12,0x00(x14)
    [yd] {!cf0} ldb.u.i y12,0x0(y14)

    [xd] { cf1} ldh.u.i x15,0x9(x17)
    [yd] {!cf1} ldh.u.i y15,0x90(y17)

    # OP_HI = 0b'001
    [xd] { cf0} lbm x0,x1(+x2)
    [xd] {!cf0} lbm x0,x1(x2+)
    [yd] { cf1} lbm y0,y1(-y2)
    [yd] {!cf1} lbm y0,y1(y2-)

    [xd] { cf0} lhm x3,x4(+x5)
    [xd] {!cf0} lhm x3,x4(x5+)
    [yd] { cf1} lhm y3,y4(+y5)
    [yd] {!cf1} lhm y3,y4(y5+)

    [xd] { cf0} lwm x9,x10(+x11)
    [xd] {!cf0} lwm x9,x10(x11+)
    [yd] { cf2} lwm y9,y10(-y11)
    [yd] {!cf2} lwm y9,y10(y11-)

    [xd] { cf0} ldm x9:x8,x10(+x11)
    [xd] {!cf0} ldm x9:x8,x10(x11+)
    [yd] { cf2} ldm y9:y8,y10(+y11)
    [yd] {!cf2} ldm y9:y8,y10(y11+)

    [xd]        lbm.u g0,g1(+g2)
    [xd]        lbm.u g0,g1(g2+)
    [yd]        lbm.u g3,g4(-g5)
    [yd]        lbm.u g3,g4(g5-)

    [xd]        lhm.u x24,x25(+x26)
    [xd]        lhm.u y24,y25(y26+)
    [yd]        lhm.u x24,x25(+x26)
    [yd]        lhm.u y24,y25(y26+)

    # K: 5-bit signed immediate(-128 to 127).
    #[xd] { cf0} lbm.i x0,-128(+x2)
    #[xd] {!cf0} lbm.i x0,127(x2+)
    #[yd] { cf1} lbm.i y0,-129(-y2)
    #[yd] {!cf1} lbm.i y0,-127(y2-)

    #[xd] { cf0} lhm.i x3,128(+x5)
    #[xd] {!cf0} lhm.i x3,126(x5+)
    #[yd] { cf1} lhm.i y3,0(+y5)
    #[yd] {!cf1} lhm.i y3,15(y5+)

    #[xd] { cf0} lwm.i x9,0xff(+x11)
    #[xd] {!cf0} lwm.i x9,0x1f(x11+)
    #[yd] { cf2} lwm.i y9,0x11(-y11)
    #[yd] {!cf2} lwm.i y9,0x2f(y11-)

    #[xd] { cf0} ldm.i x9:x8,0x0(+x11)
    #[xd] {!cf0} ldm.i x9:x8,0x10f(x11+)
    #[yd] { cf2} ldm.i y9:y8,0x02(+y11)
    #[yd] {!cf2} ldm.i y9:y8,0x3(y11+)

    #[xd]        lbm.u.i g0,125(+g2)
    #[xd]        lbm.u.i g0,300(g2+)
    #[yd]        lbm.u.i g3,100(-g5)
    #[yd]        lbm.u.i g3,1(g5-)

    #[xd]        lhm.u.i x24,-300(+x26)
    #[xd]        lhm.u.i y24,-1(y26+)
    #[yd]        lhm.u.i x24,-2(+x26)
    #[yd]        lhm.u.i y24,-125(y26+)



    # OP_HI = 0b'010
    [xd] { cf0} brr x0
    [yd] {!cf0} brr y0
    [xd]        brr g0
    [yd]        brr g0

    [xd] { cf1} brl x1
    [yd] {!cf1} brl y1
    [xd]        brl g1
    [yd]        brl g1

    [xd] { cf2} ret
    [xd] {!cf2} ret
    [xd]        ret

    [xd] { cf0} irt
    [xd] { cf1} irt
    [xd] { cf2} irt

    [xd] {!cf0} sys 0x0
    [xd] {!cf1} sys 0x1
    [xd] {!cf2} sys 100

    [xd] { cf0} nop
    [xd] { cf1} nop
    [xd] { cf2} nop

    [xd] {!cf0} smr
    [xd] {!cf1} smr
    [xd] {!cf2} smr

    [xd] { cf0} smv
    [xd] { cf1} smv
    [xd] { cf2} smv

.size t_cover_d,.-t_cover_d
.endfunc
