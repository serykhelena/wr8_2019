global dat
delete(instrfind);
dat = serial('COM7', 'BaudRate', 115200);
dat.InputBufferSize = 4096;

fopen(dat)
set(dat, 'ByteOrder', 'littleEndian')

disp 'Ok!'
temp = [-1 -1 -1 -1 -1];
A=[];
B = [];
t = 1000;
 for i = 1:t
    if (i < 6)
        A=fread(dat, [1,1], 'int16');
        temp(i) = A;
    else
        A = sum(temp)/5;
        for k = 1:4
           temp(k) = temp(k + 1); 
        end    
        temp(5) = fread(dat, [1,1], 'int16'); 
    end
 B = [B; A];
 end
fclose(dat);
sum(temp);
plot(B)
disp 'Close'