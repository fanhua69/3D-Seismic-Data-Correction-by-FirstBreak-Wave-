
        DIM PhF(4000), VF(4000)
        DIM PhS(8000), VS(8000)
       
        OPEN "Rout002.dat" FOR INPUT AS #1
        N = 0
        DO WHILE NOT EOF(1)
            N = N + 1
            INPUT #1, PhF(N), X#, Y#, VF(N)
        LOOP
        NF = N
        CLOSE #1

        OPEN "swath002.rt" FOR INPUT AS #1
        N = 0
        DO WHILE NOT EOF(1)
            N = N + 1
            INPUT #1, PhS(N), X#, Y#, VS(N)
        LOOP
        NS = N
        CLOSE #1



        OPEN "hyr.txt" FOR OUTPUT AS #1
       
        FOR I = 106 TO 115
            FOR J = 101 TO 792
                PhThis = I * 1000 + J
                PRINT PhThis;
                Founded = 0
                FOR K = 1 TO NF
                    IF PhF(K) = PhThis THEN
                        PRINT #1, PhF(K), VF(K)
                        Founded = 1
                        'PRINT "Founded"
                        EXIT FOR
                    END IF
                NEXT K

                IF Founded = 0 THEN
                    'PRINT "Not Found!"
                    FOR K = 1 TO NS
                        IF PhS(K) = PhThis THEN
                            PRINT #1, PhS(K), VS(K)
                            EXIT FOR
                        END IF
                    NEXT K
                END IF

            NEXT J
        NEXT I

        CLOSE #1

        END



