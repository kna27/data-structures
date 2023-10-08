#include "mod.h"
const int MOD1 = 17;
const long long MOD2 = __LONG_LONG_MAX__ - 24; // 9223372036854775783, I think largest prime you can fit in a signed long long

int main()
{
    Mod::set_modulus(17);
    Mod aa(15), bb(3), cc(-2);

    cout << "aa+bb: " << ((aa + bb) == Mod(1) ? 1 : 0) << '\n';    // 15 + 3 = 18 ≡ 1 (mod 17)
    cout << "aa-bb: " << ((aa - bb) == Mod(12) ? 1 : 0) << '\n';   // 15 - 3 = 12 ≡ 12 (mod 17)
    cout << "aa/bb: " << ((aa / bb) == Mod(5) ? 1 : 0) << '\n';    // 15 / 3 = 5 ≡ 5 (mod 17)
    cout << "aa^(-1): " << (aa.pwr(-1) == Mod(8) ? 1 : 0) << '\n'; // 15^(-1) ≡ 8 (mod 17)

    Mod::set_modulus(17);
    Mod o(5);
    cin >> o;
    cout << o;
    cout << "\n";

    cout << "Mod Operator Tests\n";

    Mod::set_modulus(MOD1);
    Mod a(1), b(-10), c(-21), d(99);

    // Basic tests with pre-defined values
    cout << "\tTest 1: " << (a.val() == 1 && b.val() == 7 && c.val() == 13 && d.val() == 14) << '\n';
    cout << "\tTest 2: " << (a.pwr(16) == 1 && b.pwr(16) == 1 && c.pwr(16) == 1 && d.pwr(16) == 1) << '\n';

    // Test for operator +
    Mod result1 = a + b;
    Mod result2 = 3 + b;
    cout << "\tTest 3: " << (result1.val() == ((a.val() + b.val()) % MOD1)) << '\n';
    cout << "\tTest 4: " << (result2.val() == ((3 + b.val()) % MOD1)) << '\n';

    // Test for operator -
    Mod result3 = a - c;
    Mod result4 = 10 - b;
    cout << "\tTest 5: " << (result3.val() == ((a.val() - c.val() + MOD1) % MOD1)) << '\n';
    cout << "\tTest 6: " << (result4.val() == ((10 - b.val() + MOD1) % MOD1)) << '\n';

    // Test for operator *
    Mod result5 = b * c;
    Mod result6 = 4 * d;
    cout << "\tTest 7: " << (result5.val() == ((b.val() * c.val()) % MOD1)) << '\n';
    cout << "\tTest 8: " << (result6.val() == ((4 * d.val()) % MOD1)) << '\n';

    // Test for operator /
    Mod result7 = d / a;
    cout << "\tTest 9: " << (result7.val() == ((d.val() / a.val()) % MOD1)) << '\n';

    // Test for operator ==
    bool eq_test1 = (a == b);
    bool eq_test2 = (a == 18);
    cout << "\tTest 11: " << (!eq_test1) << '\n';
    cout << "\tTest 12: " << (eq_test2) << '\n';

    // Test for operator !=
    bool neq_test1 = (a != c);
    bool neq_test2 = (b != 7);
    cout << "\tTest 13: " << (neq_test1) << '\n';
    cout << "\tTest 14: " << (!neq_test2) << '\n';

    cout << "Small Mod Test\n";
    Mod::set_modulus(MOD1);
    cout << "\tTest 1: " << (a.val() == 1 && b.val() == 7 && c.val() == 13 && d.val() == 14) << '\n';
    cout << "\tTest 2: " << (a.pwr(16) == 1 && b.pwr(16) == 1 && c.pwr(16) == 1 && d.pwr(16) == 1) << '\n';
    cout << "\tTest 3: " << (a.pwr(1234134) == 1 && b.pwr(3238475238923) == 14 && c.pwr(-1035) == 13 && d.pwr(9223372036854775807) == 11) << '\n';
    cout << "\tTest 4: " << ((-a).val() == 16 && (-b).val() == 10 && (-c).val() == 4 && (-d).val() == 3) << '\n';
    cout << "\tTest 5: " << (8 == a + b && 11 == b - c && a * d == 8 + b * c && 7 == c / d) << '\n';

    cout << "Big Mod Test\n";
    Mod::set_modulus(MOD2);
    Mod w(10), x(-2), y(-1000), z(102131248714);
    cout << "\tTest 1: " << (w.val() == 10 && x.val() == 9223372036854775781 && y.val() == 9223372036854774783 && z.val() == 102131248714) << '\n';
    cout << "\tTest 2: " << (w.pwr(9223372036854775782) == 1 && x.pwr(9223372036854775782) == 1 && y.pwr(9223372036854775782) == 1 && z.pwr(9223372036854775782) == 1) << '\n';
    cout << "\tTest 3: " << (w.pwr(9223372036854775781) == 2767011611056432735 && x.pwr(-13467274278643267) == 36926955412652977 && y.pwr(3238475238923) == 6282865623983465974 && z.pwr(9223372036854775807) == 6304613605540290559) << '\n';
    cout << "\tTest 4: " << ((-w).val() == 9223372036854775773 && (-x).val() == 2 && (-y).val() == 1000 && (-z).val() == 9223371934723527069) << '\n';
    cout << "\tTest 5: " << (12 == w - x && 998 == x - y && 2000 == x * y && 1844674417584080028 == z / w) << '\n';
}
