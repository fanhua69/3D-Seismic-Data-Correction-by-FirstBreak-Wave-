
    OPEN "swath000.rt" FOR OUTPUT AS #1
    n = 0
    FOR I = 1 TO 150
        FOR J = 101 TO 104
            n = n + 1
            Ph = J * 1000 + I
            PRINT #1, Ph; "0.0 0.0 "; SIN(n / 20) * 50
        NEXT J
    NEXT I
    CLOSE
   
    OPEN "swath000.st" FOR OUTPUT AS #1
    n = 0
    FOR I = 501 TO 510
        FOR J = 1 TO 6
            Ph = I * 1000 + J
            n = n + 1
            PRINT #1, Ph; "0.0 0.0 "; SIN(n / 20) * 50
        NEXT J
    NEXT I
    CLOSE

