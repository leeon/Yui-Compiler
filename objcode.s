1 :  BEGIN
1000 :  FUNCTION &main
1001 :  a > 4
1002 :  if  &1001 goto 1004
1003 :  goto 1016
1004 :  temp0 = b + c
1005 :  a = temp0
1006 :  a = 1
1007 :  a > 4.1
1008 :  if  &1007 goto 1013
1009 :  goto 1015
1010 :  temp1 = a + 1
1011 :  a = temp1
1012 :  goto 1007
1013 :  b = 2
1014 :  goto 1010
1015 :  goto 1020
1016 :  temp2 = b + c
1017 :  temp3 = b - c
1018 :  temp4 = temp2 * temp3
1019 :  a = temp4
1020 :  RETURN  0.5
