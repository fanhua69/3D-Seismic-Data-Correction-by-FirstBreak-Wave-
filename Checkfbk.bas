   
    DECLARE SUB DrawOneLine (BG!, n!)
    COMMON SHARED FileNo$, FBK$
    COMMON SHARED GroupInOneLine, LineInOnePage, GroupInOnePage

    GroupInOneLine = 31
    LineInOnePage = 3
    GroupInOnePage = LineInOnePage * GroupInOneLine
   
    REDIM SHARED FBK(GroupInOneLine)
   
    DO
        CLS 0
        LOCATE 1, 1: INPUT "FBK file :", F$
        IF F$ = "" THEN EXIT DO
        F$ = F$ + ".FBK"
        
        'F$ = "G01.FBK"

        OPEN F$ FOR RANDOM AS #1 LEN = 16
        FIELD #1, 8 AS FileNo$, 8 AS FBK$
        FileLen = LOF(1)
        TotalPageNumber = INT(FileLen / (GroupInOnePage * 16)) - 1   ' The first page is 0.
        IF (FileLen - TotalPageNumber * GroupInOnePage * 16 > 0) THEN
            TotalPageNumber = TotalPageNumber + 1
        END IF


        SCREEN 12
        VIEW (1, 30)-(638, 460), 0, 10
        WINDOW SCREEN (0, -100)-(GroupInOnePage + 2, 2100)
        
        Page = 0
        DO
            CLS
            LOCATE 1, 1: PRINT "       Page: "; Page, F$, "Total Page:", TotalPageNumber
           
            FOR I = 1 TO LineInOnePage
                BeginGroup = Page * GroupInOnePage + GroupInOneLine * (I - 1)
                CALL DrawOneLine(BeginGroup, I)
            NEXT I

            A$ = INPUT$(1)
            A$ = UCASE$(A$)
            IF A$ = "P" THEN
                Page = Page - 1
                IF Page < 0 THEN Page = 0
            ELSEIF A$ = "S" THEN
                LOCATE 1, 1: PRINT STRING$(80, " ")
                LOCATE 1, 1: INPUT "Page Number:", Page
            ELSEIF A$ = CHR$(27) THEN
                EXIT DO
            ELSE
                Page = Page + 1
            END IF

            IF Page > TotalPageNumber THEN
                BEEP
                Page = TotalPageNumber
            END IF
        LOOP
        CLOSE
    LOOP
    END

SUB DrawOneLine (BG, n) STATIC
        FOR I = 1 TO GroupInOneLine
            GET #1, BG + I
            FBK(I) = CVD(FBK$)
        NEXT I

        BX = (n - 1) * GroupInOneLine
        FOR I = 1 TO GroupInOneLine - 1
            LINE (I + BX, FBK(I))-(I + BX + 1, FBK(I + 1)), 12
            CIRCLE (I + BX, FBK(I)), .5, 10
        NEXT I
        CIRCLE (GroupInOneLine + BX, FBK(GroupInOneLine)), .5, 10
END SUB

