
        DIM Ph(1500), X#(1500), Y#(1500), V(1500)
        OPEN "sout002.dat" FOR INPUT AS #1
        FOR I = 1 TO 1500
                INPUT #1, Ph(I), X#(I), Y#(I), V(I)
        NEXT I
        CLOSE #1

        OPEN "s.dat" FOR OUTPUT AS #1
        OPEN "swath002.st" FOR INPUT AS #2
        FOR I = 1 TO 20
                FOR J = 1 TO 75
                        INPUT #2, Ph, X#, Y#, V
                        PRINT #1, Ph, X#, Y#, V
                NEXT J
                FOR J = 1 TO 75
                        INPUT #2, Ph, X#, Y#, V
                        N = (J - 1) * 20 + I
                        PRINT #1, Ph(N), X#(N), Y#(N), V(N)
                NEXT J
        NEXT I
        CLOSE
        END



