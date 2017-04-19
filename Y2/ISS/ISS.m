%1.)
[s, Fs] = audioread ('xbazik00.wav'); s = s';

%2.)
y = fft(s);
show = abs(y(1:Fs/2+1));
k = 0:Fs/2;
plot(k,show);xlabel('f');

%3.)
c = find(show==max(show));

%4.)
A = [1.0000,  0.2289,   0.4662];
B = [0.2324, -0.4112, 0.2324];
zplane (B,A); 
p = roots(A); 
if (isempty(p) | abs(p) < 1) 
  disp('Stable')
else
  disp('Unstable')
end

%5.)
H = freqz(B,A,256); f=(0:255) / 256 * Fs / 2; 
plot (f,abs(H)); grid; xlabel('f'); ylabel('|H(f)|')

%6.)
fils = filter(B,A,s);
fily = fft(fils);
filshow = abs(fily(1:Fs/2+1));
filk = 0:Fs/2;
plot(filk,filshow);xlabel('f');

%7.)
filc = find(filshow==max(filshow));

%8.)
t = 0.0000001:1/Fs:0.02;
x_rec = square(2*pi*2000*t);
for a = 1:(16000-319) 
  xs = s(a:319+a).*x_rec; 
  coef(a) = sum (xs);
end
rec = find(coef==max(coef));

%9.)
for a = -50:50
  for b = 1:(Fs-1)
    if a+b < 1 || a+b > Fs
       u(b) = s(b);
    else
       u(b) = s(b)*s(b+a);
    end
  end
  R(a+51) = 1/Fs*sum(u); 
end
x = -50:50;
plot(x,R); xlabel('k'); ylabel('R[k]')

%10.)
R(10+51)

%11.)
k = 10;
data1 = s(1:end-k); data2 = s(k+1:end);

x = linspace(-1,1,100); 

L = length(x); 
N = length(data1); 
h = zeros(L,L); out = 0;
xcol = x(:); bigx = repmat(xcol,1,N);
yr = data1(:)'; bigy = repmat(yr,L,1);
[dummy,ind1] = min(abs(bigy - bigx)); 
yr = data2(:)'; bigy = repmat(yr,L,1);
[dummy,ind2] = min(abs(bigy - bigx)); 
for ii=1:N,
  d1 = ind1(ii);   d2 = ind2(ii); 
  h(d1,d2) = h(d1,d2) + 1; 
end
imagesc (x,x,h); axis xy; colorbar

%12.)
surf = (x(2) - x(1))^2;
p = h / N / surf;
check = sum(sum (p)) * surf

%13.) 
x = x(:); X1 = repmat(x,1,L); 
x = x'; X2 = repmat(x,L,1); 
r = sum(sum (X1 .* X2 .* p)) * surf


