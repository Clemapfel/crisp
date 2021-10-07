/*
 * Copyright (c) 2003, 2007-14 Matteo Frigo
 * Copyright (c) 2003, 2007-14 Massachusetts Institute of Technology
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

/* This file was automatically generated --- DO NOT EDIT */
/* Generated on Thu Dec 10 07:04:10 EST 2020 */

#include "dft/codelet-dft.h"

#if defined(ARCH_PREFERS_FMA) || defined(ISA_EXTENSION_PREFERS_FMA)

/* Generated by: ../../../genfft/gen_twiddle.native -fma -compact -variables 4 -pipeline-latency 4 -n 5 -name t1_5 -include dft/scalar/t.h */

/*
 * This function contains 40 FP additions, 34 FP multiplications,
 * (or, 14 additions, 8 multiplications, 26 fused multiply/add),
 * 31 stack variables, 4 constants, and 20 memory accesses
 */
#include "dft/scalar/t.h"

static void t1_5(R *ri, R *ii, const R *W, stride rs, INT mb, INT me, INT ms)
{
     DK(KP951056516, +0.951056516295153572116439333379382143405698634);
     DK(KP559016994, +0.559016994374947424102293417182819058860154590);
     DK(KP618033988, +0.618033988749894848204586834365638117720309180);
     DK(KP250000000, +0.250000000000000000000000000000000000000000000);
     {
	  INT m;
	  for (m = mb, W = W + (mb * 8); m < me; m = m + 1, ri = ri + ms, ii = ii + ms, W = W + 8, MAKE_VOLATILE_STRIDE(10, rs)) {
	       E T1, TM, T7, Tx, Td, Tz, Te, TJ, Tk, TC, Tq, TE, Tr, TK;
	       T1 = ri[0];
	       TM = ii[0];
	       {
		    E T3, T6, T4, Tw, T9, Tc, Ta, Ty, T2, T8, T5, Tb;
		    T3 = ri[WS(rs, 1)];
		    T6 = ii[WS(rs, 1)];
		    T2 = W[0];
		    T4 = T2 * T3;
		    Tw = T2 * T6;
		    T9 = ri[WS(rs, 4)];
		    Tc = ii[WS(rs, 4)];
		    T8 = W[6];
		    Ta = T8 * T9;
		    Ty = T8 * Tc;
		    T5 = W[1];
		    T7 = FMA(T5, T6, T4);
		    Tx = FNMS(T5, T3, Tw);
		    Tb = W[7];
		    Td = FMA(Tb, Tc, Ta);
		    Tz = FNMS(Tb, T9, Ty);
		    Te = T7 + Td;
		    TJ = Tx + Tz;
	       }
	       {
		    E Tg, Tj, Th, TB, Tm, Tp, Tn, TD, Tf, Tl, Ti, To;
		    Tg = ri[WS(rs, 2)];
		    Tj = ii[WS(rs, 2)];
		    Tf = W[2];
		    Th = Tf * Tg;
		    TB = Tf * Tj;
		    Tm = ri[WS(rs, 3)];
		    Tp = ii[WS(rs, 3)];
		    Tl = W[4];
		    Tn = Tl * Tm;
		    TD = Tl * Tp;
		    Ti = W[3];
		    Tk = FMA(Ti, Tj, Th);
		    TC = FNMS(Ti, Tg, TB);
		    To = W[5];
		    Tq = FMA(To, Tp, Tn);
		    TE = FNMS(To, Tm, TD);
		    Tr = Tk + Tq;
		    TK = TC + TE;
	       }
	       {
		    E Tu, Ts, Tt, TG, TI, TA, TF, TH, Tv;
		    Tu = Te - Tr;
		    Ts = Te + Tr;
		    Tt = FNMS(KP250000000, Ts, T1);
		    TA = Tx - Tz;
		    TF = TC - TE;
		    TG = FMA(KP618033988, TF, TA);
		    TI = FNMS(KP618033988, TA, TF);
		    ri[0] = T1 + Ts;
		    TH = FNMS(KP559016994, Tu, Tt);
		    ri[WS(rs, 2)] = FNMS(KP951056516, TI, TH);
		    ri[WS(rs, 3)] = FMA(KP951056516, TI, TH);
		    Tv = FMA(KP559016994, Tu, Tt);
		    ri[WS(rs, 4)] = FNMS(KP951056516, TG, Tv);
		    ri[WS(rs, 1)] = FMA(KP951056516, TG, Tv);
	       }
	       {
		    E TO, TL, TN, TS, TU, TQ, TR, TT, TP;
		    TO = TJ - TK;
		    TL = TJ + TK;
		    TN = FNMS(KP250000000, TL, TM);
		    TQ = T7 - Td;
		    TR = Tk - Tq;
		    TS = FMA(KP618033988, TR, TQ);
		    TU = FNMS(KP618033988, TQ, TR);
		    ii[0] = TL + TM;
		    TT = FNMS(KP559016994, TO, TN);
		    ii[WS(rs, 2)] = FMA(KP951056516, TU, TT);
		    ii[WS(rs, 3)] = FNMS(KP951056516, TU, TT);
		    TP = FMA(KP559016994, TO, TN);
		    ii[WS(rs, 1)] = FNMS(KP951056516, TS, TP);
		    ii[WS(rs, 4)] = FMA(KP951056516, TS, TP);
	       }
	  }
     }
}

static const tw_instr twinstr[] = {
     { TW_FULL, 0, 5 },
     { TW_NEXT, 1, 0 }
};

static const ct_desc desc = { 5, "t1_5", twinstr, &GENUS, { 14, 8, 26, 0 }, 0, 0, 0 };

void X(codelet_t1_5) (planner *p) {
     X(kdft_dit_register) (p, t1_5, &desc);
}
#else

/* Generated by: ../../../genfft/gen_twiddle.native -compact -variables 4 -pipeline-latency 4 -n 5 -name t1_5 -include dft/scalar/t.h */

/*
 * This function contains 40 FP additions, 28 FP multiplications,
 * (or, 26 additions, 14 multiplications, 14 fused multiply/add),
 * 29 stack variables, 4 constants, and 20 memory accesses
 */
#include "dft/scalar/t.h"

static void t1_5(R *ri, R *ii, const R *W, stride rs, INT mb, INT me, INT ms)
{
     DK(KP250000000, +0.250000000000000000000000000000000000000000000);
     DK(KP559016994, +0.559016994374947424102293417182819058860154590);
     DK(KP587785252, +0.587785252292473129168705954639072768597652438);
     DK(KP951056516, +0.951056516295153572116439333379382143405698634);
     {
	  INT m;
	  for (m = mb, W = W + (mb * 8); m < me; m = m + 1, ri = ri + ms, ii = ii + ms, W = W + 8, MAKE_VOLATILE_STRIDE(10, rs)) {
	       E T1, TE, Tu, Tx, TJ, TI, TB, TC, TD, Tc, Tn, To;
	       T1 = ri[0];
	       TE = ii[0];
	       {
		    E T6, Ts, Tm, Tw, Tb, Tt, Th, Tv;
		    {
			 E T3, T5, T2, T4;
			 T3 = ri[WS(rs, 1)];
			 T5 = ii[WS(rs, 1)];
			 T2 = W[0];
			 T4 = W[1];
			 T6 = FMA(T2, T3, T4 * T5);
			 Ts = FNMS(T4, T3, T2 * T5);
		    }
		    {
			 E Tj, Tl, Ti, Tk;
			 Tj = ri[WS(rs, 3)];
			 Tl = ii[WS(rs, 3)];
			 Ti = W[4];
			 Tk = W[5];
			 Tm = FMA(Ti, Tj, Tk * Tl);
			 Tw = FNMS(Tk, Tj, Ti * Tl);
		    }
		    {
			 E T8, Ta, T7, T9;
			 T8 = ri[WS(rs, 4)];
			 Ta = ii[WS(rs, 4)];
			 T7 = W[6];
			 T9 = W[7];
			 Tb = FMA(T7, T8, T9 * Ta);
			 Tt = FNMS(T9, T8, T7 * Ta);
		    }
		    {
			 E Te, Tg, Td, Tf;
			 Te = ri[WS(rs, 2)];
			 Tg = ii[WS(rs, 2)];
			 Td = W[2];
			 Tf = W[3];
			 Th = FMA(Td, Te, Tf * Tg);
			 Tv = FNMS(Tf, Te, Td * Tg);
		    }
		    Tu = Ts - Tt;
		    Tx = Tv - Tw;
		    TJ = Th - Tm;
		    TI = T6 - Tb;
		    TB = Ts + Tt;
		    TC = Tv + Tw;
		    TD = TB + TC;
		    Tc = T6 + Tb;
		    Tn = Th + Tm;
		    To = Tc + Tn;
	       }
	       ri[0] = T1 + To;
	       ii[0] = TD + TE;
	       {
		    E Ty, TA, Tr, Tz, Tp, Tq;
		    Ty = FMA(KP951056516, Tu, KP587785252 * Tx);
		    TA = FNMS(KP587785252, Tu, KP951056516 * Tx);
		    Tp = KP559016994 * (Tc - Tn);
		    Tq = FNMS(KP250000000, To, T1);
		    Tr = Tp + Tq;
		    Tz = Tq - Tp;
		    ri[WS(rs, 4)] = Tr - Ty;
		    ri[WS(rs, 3)] = Tz + TA;
		    ri[WS(rs, 1)] = Tr + Ty;
		    ri[WS(rs, 2)] = Tz - TA;
	       }
	       {
		    E TK, TL, TH, TM, TF, TG;
		    TK = FMA(KP951056516, TI, KP587785252 * TJ);
		    TL = FNMS(KP587785252, TI, KP951056516 * TJ);
		    TF = KP559016994 * (TB - TC);
		    TG = FNMS(KP250000000, TD, TE);
		    TH = TF + TG;
		    TM = TG - TF;
		    ii[WS(rs, 1)] = TH - TK;
		    ii[WS(rs, 3)] = TM - TL;
		    ii[WS(rs, 4)] = TK + TH;
		    ii[WS(rs, 2)] = TL + TM;
	       }
	  }
     }
}

static const tw_instr twinstr[] = {
     { TW_FULL, 0, 5 },
     { TW_NEXT, 1, 0 }
};

static const ct_desc desc = { 5, "t1_5", twinstr, &GENUS, { 26, 14, 14, 0 }, 0, 0, 0 };

void X(codelet_t1_5) (planner *p) {
     X(kdft_dit_register) (p, t1_5, &desc);
}
#endif
