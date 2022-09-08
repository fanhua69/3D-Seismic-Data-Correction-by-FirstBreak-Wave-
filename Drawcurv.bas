DECLARE SUB InData ()
DECLARE SUB CalcData ()
DECLARE SUB DrawData ()

    DIM SHARED X(500), AF(500), AB(500), ABB(500)
    COMMON SHARED Total

    InData
    CalcData
    DrawData
   
    END


    '==========  Draw lines ==========='









SUB CalcData STATIC
    '====== Calculate the data.
    ' delete the data that is small the average .
    FOR I = 1 TO 500
        IF ABS(X(I)) < Total THEN
            X(I) = 0
        END IF
    NEXT I

    ' calcua;ltre the average data before one point.
    Half = 20
    FOR I = 41 TO 500
        AF(I) = 0
        FOR J = I TO I - Half STEP -1
            AF(I) = AF(I) + ABS(X(J))
        NEXT J
        AF(I) = AF(I) / Half
    NEXT I

    ' calcua;lete the data that is after one point .
    FOR I = 1 TO 460
        AB(I) = 0
        FOR J = I TO I + Half STEP 1
            AB(I) = AB(I) + ABS(X(J))
        NEXT J
        AB(I) = AB(I) / Half
    NEXT I

    ' calcua;tee the value of before one point and after thr point.
    FOR I = 41 TO 460
        IF AB(I) = 0 THEN
            ABB(I) = 0
        ELSE
            ABB(I) = AF(I) / AB(I)
        END IF
    NEXT I

END SUB

SUB DrawData STATIC
    SCREEN 12
    WINDOW SCREEN (0, 0)-(100, 500)
   
    ' Darw the line.
    FOR I = 1 TO 500
        LINE (20, I)-(20 + X(I) / 20000000, I)
    NEXT I

    ' Draw front aver line.
    FOR I = 1 TO 460
        LINE (40, I)-(40 + AF(I) / 20000000, I)
    NEXT I

    ' DSraw back aver line.
    FOR I = 41 TO 500
        LINE (60, I)-(60 + AB(I) / 20000000, I), 12
    NEXT I

    ' Draw f /b .
    FOR I = 41 TO 460
        LINE (80, I)-(80 + ABB(I) / .05, I)
    NEXT I
   

END SUB

SUB InData STATIC
 
    '====  input the data.
    OPEN "1" FOR INPUT AS #1
    Total = 0
    FOR I = 1 TO 500
        INPUT #1, X(I)
        Total = Total + X(I)
    NEXT I
    Total = Total / 500

END SUB

