.file "t1.s"

.text
.align 4

.func fu_a
.type fu_a,@function

fu_a:
    [xa]        mkl x0, 0x1234
    [xa]        mkh x0, 0x9876
    [xa]        mkl x1, 0x5678
    [xa]        mkh x1, 0x1489
    [ya]        mkl y20,1000
    [ya]        mkh y21,2000

    # FU = 0b'00, OPH = 0b'000
    [xa] { cf0} add     x0,x1,x2
    [xa] { cf0} add.b.4 x0,x1,x2
    [xa] { cf0} add.h.2 x0,x1,x2
    [xa] { cf0} add.h.4 x1:x0,x3:x2,x5:x4
    [xa] { cf0} add.w.2 x1:x0,x3:x2,x5:x4
    [xa] { cf0} add.i   x0,x1,127

    # FU = 0b'00, OPH = 0b'001
    [xa] { cf1} and     x10,x11,x12
    [xa] { cf1} and.b.4 x10,x11,x12
    [xa] { cf1} and.h.2 x10,x11,x12
    [xa] { cf1} and.h.4 x11:x10,x13:x12,x15:x14
    [xa] { cf1} and.w.2 x11:x10,x13:x12,x15:x14
    [xa] { cf1} and.i   x10,x11,127

    # FU = 0b'00, OPH = 0b'010
    [xa] { cf2} tgt     x20,x21,x22
    [xa] { cf2} tgt.i   x20,x21,100
    [xa] { cf2} tgt.u   x20,x21,x22
    [xa] { cf2} tgt.u.i x20,x21,100

    # FU = 0b'00, OPH = 0b'011
    [ya]        gto.b.4   y0,y1,y2
    [ya]        gto.h.2   y0,y1,y2
    [ya]        gto.h.4   y1:y0,y3:y2,y5:y4
    [ya]        gto.w.2   y1:y0,y3:y2,y5:y4
    [ya]        gta.b.4   y0,y1,y2
    [ya]        gta.h.2   y0,y1,y2
    [ya]        gta.h.4   y1:y0,y3:y2,y5:y4
    [ya]        gta.w.2   y1:y0,y3:y2,y5:y4
    [ya]        gto.u.b.4 y0,y1,y2
    [ya]        gto.u.h.2 y0,y1,y2
    [ya]        gto.u.h.4 y1:y0,y3:y2,y5:y4
    [ya]        gto.u.w.2 y1:y0,y3:y2,y5:y4
    [ya]        gta.u.b.4 y0,y1,y2
    [ya]        gta.u.h.2 y0,y1,y2
    [ya]        gta.u.h.4 y1:y0,y3:y2,y5:y4
    [ya]        gta.u.w.2 y1:y0,y3:y2,y5:y4

    # FU = 0b'00, OPH = 0b'100
    [ya] {!cf0} lsl     y10,y11,y12
    [ya] {!cf0} lsl.b.4 y10,y11,y12
    [ya] {!cf0} lsl.h.2 y10,y11,y12
    [ya] {!cf0} lsl.h.4 y11:y10,y13:y12,y15:y14
    [ya] {!cf0} lsl.w.2 y11:y10,y13:y12,y15:y14
    [ya] {!cf0} lsl.i   y10,y11,20

    # FU = 0b'00, OPH = 0b'101
    [ya] {!cf1} mkl y20,1000
    [ya] {!cf1} mkh y21,2000

    # FU = 0b'00, OPH = 0b'110
    [ya] {!cf2} zip.b.4 g1:g0,g2,g3
    [ya] {!cf2} zip.h.2 g1:g0,g2,g3
    [ya] {!cf2} zip.h.4 g3:g2:g1:g0,g3:g2,g5:g4
    [ya] {!cf2} zip.w.2 g3:g2:g1:g0,g3:g2,g5:g4

    # FU = 0b'01, OPH = 0b'000
    [ya] { cf0} add.sp   g0,g1,g2
    [ya] { cf0} add.dp   g1:g0,g3:g2,g5:g4
    [ya] { cf0} add.sp.2 g1:g0,g3:g2,g5:g4

    # FU = 0b'01, OPH = 0b'010
    [ya] { cf1} tgt.sp   g3,g4,g5
    [ya] { cf1} tgt.dp   g3:g2,g5:g4,g7:g6

    # FU = 0b'01, OPH = 0b'011
    [ya] { cf2} gto.sp.2 g3:g2,g5:g4,g7:g6
    [ya] { cf2} gta.sp.2 g3:g2,g5:g4,g7:g6

.size fu_a,.-fu_a
.endfunc


