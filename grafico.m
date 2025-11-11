function grafico
  x=[1,3,5,7,9];
  y=[1,2.88,4.96,6.89,8.91];
  figure(1);
  plot(x,y,"-*b",x,x,"--r");
  legend("SpeedUp(N)","SpeedUp Lineal",'location','northwest')
  title("SpeedUp en función del número de nodos");
  xlabel("N");
  grid on;
  print("grafSpeedUp.png");
endfunction
