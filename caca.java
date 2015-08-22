import java.util.*;
import java.lang.*;
import java.io.*;
import java.awt.Point;

public class Main {
	static BufferedReader br = new BufferedReader(new InputStreamReader(
			System.in));
	static PrintWriter out = new PrintWriter(System.out);
	static StringTokenizer st = new StringTokenizer("");

	static String next() {
		try {
			while (!st.hasMoreTokens()) {
				String s = br.readLine();
				if (s == null)
					return null;
				st = new StringTokenizer(s);
			}
			return st.nextToken();
		} catch (Exception e) {
			return null;
		}
	}

	public static void main(String[] args) throws java.lang.Exception {

		while (true) {
			String s = next();
			if (s == null)
				break;
			N = Integer.parseInt(s);

			h = new long[N + 1];
			c = new long[N + 1];

			for (int k = 1; k <= N; k++)
				h[k] = Integer.parseInt(next());

			for (int k = 1; k <= N; k++)
				c[k] = Integer.parseInt(next());

			// Formula is dp[i] = min(tax[j] * height[i] + dp[j] | j = 1 .. i -
			// 1)
			// Here tax[j] is considered as m value, and dp[j] as b value in
			// line equation y = m * x + b
			long[] dp = new long[N + 1];
			mvals = new long[N + 1];
			bvals = new long[N + 1];
			len = cur = 0;
			for (int i = 1; i <= N; i++) {
				if (i == 1) {
					dp[i] = 0;
				} else {
					setCur(h[i]);
					dp[i] = mvals[cur] * h[i] + bvals[cur];
				}
				add(c[i], dp[i]);
			}

			out.println(dp[N]);
		}
		//
		out.close();
		System.exit(0);
	}

	static int N;
	static long[] h, c; // height, cost

	static long[] mvals, bvals;
	static int len = 0;
	static int cur = 0;

	// Suppose the last 3 lines added are : (l1, l2, l3)
	// Line l2 becomes irrelevant, if l1/l3 x-intersection is to the left of
	// l1/l2 x-intersection
	static boolean bad(long m1, long b1, long m2, long b2, long m3, long b3) {
		// Cast to double to avoid long long overflow
		return 1.0 * (b1 - b3) * (m2 - m1) < 1.0 * (b1 - b2) * (m3 - m1);
	}

	static void add(long m, long b) {
		while (len >= 2
				&& bad(mvals[len - 2], bvals[len - 2], mvals[len - 1],
						bvals[len - 1], m, b)) {
			len--;
		}
		mvals[len] = m;
		bvals[len++] = b;
	}

	static void setCur(long x) {
		if (cur > len - 1)
			cur = len - 1;
		// Best-line pointer goes to the right only when queries are
		// non-decreasing (x argument grows)
		while (cur < len - 1
				&& 1.0 * mvals[cur + 1] * x + bvals[cur + 1] <= 1.0
						* mvals[cur] * x + bvals[cur])
			cur++;
	}
}
