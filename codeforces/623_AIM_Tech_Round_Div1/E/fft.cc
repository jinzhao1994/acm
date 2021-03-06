#include <iostream>
#include <complex>
#include <algorithm>
#include <cmath>
#include <cstring>

using namespace std;

//typedef complex<long double> Complex;

struct Complex {
	long double r, i;
	Complex(long double r = 0, long double i = 0):r(r), i(i) {
	}
};
Complex operator + (const Complex &a, const Complex &b) {
	return Complex(a.r + b.r, a.i + b.i);
}
Complex operator - (const Complex &a, const Complex &b) {
	return Complex(a.r - b.r, a.i - b.i);
}
Complex operator * (const Complex &a, const Complex &b) {
	return Complex(a.r * b.r - a.i * b.i, a.r * b.i + a.i * b.r);
}
Complex operator / (const Complex &a, long double b) {
	return Complex(a.r / b, a.i / b);
}

const int mod = 1000000007;
const int K = 32768;
const int K2 = K * 2;
const long double PI = acos((long double)-1.0);

struct DP {
	int v[K], n;
	DP() {
		memset(v, 0, sizeof(v));
	}
};

int fact[K];

void FFT(Complex a[], int n, int oper) {
	for (int i = 1, j = 0; i < n; i++) {
		for (int s = n; j ^= s >>= 1, ~j & s; );
		if (i < j) swap(a[i], a[j]);
	}
	for (int m = 1; m < n; m *= 2) {
		long double p = PI / m * oper;
		Complex w = Complex(cos(p), sin(p));
		for (int i = 0; i < n; i += m * 2) {
			Complex unit = 1;
			for (int j = 0; j < m; j++) {
				Complex &x = a[i + j + m], &y = a[i + j], t = unit * x;
				x = y - t;
				y = y + t;
				unit = unit * w;
			}
		}
	}
	if (oper == -1) for (int i = 0; i < n; i++) a[i] = a[i] / n;
}
int power(int x, int n) {
	int ans = 1;
	while (n) {
		if (n & 1) {
			ans = (long long)ans * x % mod;
		}
		x = (long long)x * x % mod;
		n >>= 1;
	}
	return ans;
}
DP operator * (DP a, DP b) {
	for (int i = 0; i < K; i++) {
		a.v[i] = (long long)a.v[i] * power(2, b.n * i) % mod;
	}
	const int base = sqrt(mod);
	static Complex a0[K2], a1[K2], b0[K2], b1[K2];
	static Complex c00[K2], c01[K2], c11[K2];
	for (int i = 0; i < K2; i++) {
		a0[i] = a1[i] = b0[i] = b1[i] = 0;
	}
	for (int i = 0; i < K; i++) {
		a0[i] = a.v[i] % base;
		a1[i] = a.v[i] / base;
		b0[i] = b.v[i] % base;
		b1[i] = b.v[i] / base;
	}
	FFT(a0, K2, 1);
	FFT(a1, K2, 1);
	FFT(b0, K2, 1);
	FFT(b1, K2, 1);
	for (int i = 0; i < K2; i++) {
		c00[i] = a0[i] * b0[i];
		c01[i] = (a0[i] + a1[i]) * (b0[i] + b1[i]);
		c11[i] = a1[i] * b1[i];
	}
	FFT(c00, K2, -1);
	FFT(c01, K2, -1);
	FFT(c11, K2, -1);
	DP ans;
	ans.n = a.n + b.n;
	for (int i = 0; i < K; i++) {
		long long k00 = (long long)(c00[i].r + 0.5) % mod;
		long long k01 = (long long)(c01[i].r + 0.5) % mod;
		long long k11 = (long long)(c11[i].r + 0.5) % mod;
		ans.v[i] = ((k11 * base + (k01 - k00 - k11) % mod + mod) % mod * base + k00) % mod;
	}
	return ans;
}
DP cal(int n) {
	DP x, ans;
	ans.n = 0;
	ans.v[0] = 1;
	x.n = 1;
	for (int i = 1; i < K; i++) {
		x.v[i] = power(fact[i], mod - 2);
	}
	while (n) {
		if (n & 1) {
			ans = ans * x;
		}
		x = x * x;
		n >>= 1;
	}
	return ans;
}

int main() {
	fact[0] = 1;
	for (int i = 1; i < K; i++) {
		fact[i] = (long long)fact[i - 1] * i % mod;
	}
	long long n;
	int k;
	cin >> n >> k;
	if (n > k) {
		cout << 0 << endl;
		return 0;
	}
	DP dp = cal(n);
	int ans = 0;
	int c = 1;
	for (int i = 0; i <= k; i++) {
		ans = (ans + (long long)dp.v[i] * fact[i] % mod * c) % mod;
		c = (long long)c * (k - i) % mod * power(i + 1, mod - 2) % mod;
	}
	cout << ans << endl;
	return 0;
}
