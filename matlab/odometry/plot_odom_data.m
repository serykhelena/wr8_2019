lpf = [];
no_lpf = [];
for i=1:2:length(all_data)-1
   no_lpf = [no_lpf; all_data(i, 1)];
   lpf = [lpf; all_data(i+1, 1)];
end

plot(lpf)
grid on
hold on
plot(no_lpf)
    