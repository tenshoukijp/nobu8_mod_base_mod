/*
0085AFCC  D0 3B 52 00 50 40 52 00 B0 76 5E 00 D0 76 5E 00  –;R.P@R.∞v^.–v^.
0085AFDC  A0 7D 5E 00 B0 40 52 00 D0 40 52 00 F0 40 52 00  †}^.∞@R.–@R.@R.
0085AFEC  6F 70 65 6E 69 6E 67 2E 61 76 69 00 F0 29 88 00  opening.avi.ÅEÅE
0085AFFC  90 7F 5E 00 B0 3B 52 00 D0 3B 52 00 50 40 52 00  ÅE^.∞;R.–;R.P@R.

005E7795   6A 00            PUSH 0
005E7797   68 ECAF8500      PUSH Nobunaga.0085AFEC                                      ; ASCII "opening.avi"
005E779C   8B45 F0          MOV EAX,DWORD PTR SS:[EBP-10]
005E779F   50               PUSH EAX

0084DC4C  90 40 52 00 B0 40 52 00 D0 40 52 00 F0 40 52 00  ê@R.∞@R.–@R.@R.
0084DC5C  65 6E 64 69 6E 67 2E 61 76 69 00 00 11 01 00 00  ending.avi....
0084DC6C  00 00 00 00 6E 14 00 00 6E 14 00 00 38 00 00 00  ....n..n..8...

0050339F   6A 01            PUSH 1
005033A1   68 5CDC8400      PUSH Nobunaga.0084DC5C                                      ; ASCII "ending.avi"
005033A6   8B45 F0          MOV EAX,DWORD PTR SS:[EBP-10]
005033A9   50               PUSH EAX
*/