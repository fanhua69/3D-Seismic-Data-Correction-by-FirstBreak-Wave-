

        OPEN "swath003.our" FOR INPUT AS #1
        DIM Ph(4000), X#(4000), Y#(4000), V(4000)
        i = 0
        DO WHILE NOT EOF(1)
                i = i + 1
                INPUT #1, Ph(i), X#(i), Y#(i), V(i)
        LOOP
        CLOSE #1
        nFbk = i

        OPEN "swath003.snr" FOR INPUT AS #1
        DIM PhS(7000), Xs#(7000), Ys#(7000), Vs(7000)
        i = 0
        DO WHILE NOT EOF(1)
                i = i + 1
                INPUT #1, PhS(i), Xs#(i), Ys#(i), Vs(i)
        LOOP
        CLOSE #1
        nSnd = i
       
       
        OPEN "swath003all.our" FOR OUTPUT AS #1
       
        FOR i = 106 TO 115
                FOR j = 393 TO 792
                        PhNew = i * 1000 + j
                        n = -1
                        FOR k = 1 TO nFbk
                                IF PhNew = Ph(k) THEN
                                        n = k
                                        EXIT FOR
                                END IF
                        NEXT k

                        IF n <> -1 THEN
                                PRINT #1, Ph(n); X#(n); Y#(n); V(n)
                        ELSE
                                FOR k = 1 TO nSnd
                                        IF PhNew = PhS(k) THEN
                                                n = k
                                                EXIT FOR
                                        END IF
                                NEXT k
                                IF n = -1 THEN
                                        PRINT "Error:"; PhNew
                                ELSE
                                        PRINT #1, PhS(n); Xs#(n); Ys#(n); Vs(n)
                                END IF
                        END IF
                NEXT j
        NEXT i
        CLOSE #1
        END













