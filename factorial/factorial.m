answer = 1;
N = 50000;
    
for i = 2:N
    answer = answer * i;
endfor

answer

disp([num2str(N),'! has been finished!'])