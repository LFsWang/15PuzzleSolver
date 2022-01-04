#include <bits/stdc++.h>
using namespace std;
using ull = unsigned long long;

class Board
{
public:
    Board() : s(0) {}

    ull get(int i, int j) const
    {
        auto pos = (i * 4 + j) * width;
        return colmask & (s >> pos);
    }

    void set(int i, int j, ull v)
    {
        auto pos = (i * 4 + j) * width;
        ull mask = (~0) ^ (colmask << pos);
        s = (s & mask) | ((v & colmask) << pos);
    }

    ull raw() const
    {
        return s;
    }

    void clear()
    {
        s = 0;
    }

    void show() const
    {
        for (int i = 0; i < 4; ++i)
        {
            for (int j = 0; j < 4; ++j)
            {
                cout << setw(3) << get(i, j) << ' ';
            }
            cout << '\n';
        }
        cout << flush;
    }

    tuple<int, int> loc(ull x) const
    {
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                if (get(i, j) == x)
                    return make_tuple(i, j);
        __builtin_unreachable();
    }

    static ull warp(ull v)
    {
        return v == 0 ? 15 : v - 1;
    }

    ull predict() const
    {
        ull dist = 0;
        for (int i = 0; i < 4; ++i)
        {
            for (int j = 0; j < 4; ++j)
            {
                int tX = warp(get(i, j)) / 4;
                int tY = warp(get(i, j)) % 4;

                dist += abs(tX - i) + abs(tY - j);
            }
        }
        return dist;
    }

    bool isSolvable() const
    {
        vector<bool> hit(16, false);

        for (int i = 0; i < 16; ++i)
        {
            if (get(i / 4, i % 4) >= 16)
                return false;

            if (!hit[get(i / 4, i % 4)])
                hit[get(i / 4, i % 4)] = true;
            else
                return false;
        }

        return true;
    }

    bool operator==(const Board &b) const
    {
        return raw() == b.raw();
    }

private:
    inline static ull width = 4;
    inline static ull colmask = 0xFULL;
    ull s;
};

template <>
struct std::hash<Board>
{
    std::size_t operator()(Board const &s) const noexcept
    {
        return std::hash<ull>()(s.raw());
    }
};

using Pack = pair<Board, int>;
bool _gFast;
inline bool operator<(const Pack &a, const Pack &b)
{
    if (_gFast)
        return a.first.predict() > b.first.predict();
    return a.first.predict() + a.second > b.first.predict() + b.second;
}

unordered_map<Board, ull> LastStep;
unordered_map<Board, ull> Dist;
unordered_map<Board, bool> Used;

bool solve(Board init, Board target, bool fast = false)
{
    priority_queue<Pack> pq;
    _gFast = fast;

    pq.emplace(init, 0);
    LastStep[init] = 0;
    Dist[init] = 0;

    while (pq.size() > 0)
    {
        Board b;
        tie(b, ignore) = pq.top();
        pq.pop();

        if (b == target)
            return true;
        if (Used.find(b) != Used.end())
            continue;
        Used[b] = true;

        int x = 0, y = 0;
        tie(x, y) = b.loc(0);

        int dx[] = {1, -1, 0, 0};
        int dy[] = {0, 0, 1, -1};
        for (int d = 0; d < 4; ++d)
        {
            int nx = x + dx[d];
            int ny = y + dy[d];

            if (nx < 0 || 4 <= nx)
                continue;
            if (ny < 0 || 4 <= ny)
                continue;

            Board tmp = b;
            tmp.set(x, y, b.get(nx, ny));
            tmp.set(nx, ny, 0);

            if (LastStep.find(tmp) == LastStep.end())
            {
                LastStep[tmp] = b.get(nx, ny);
                Dist[tmp] = Dist[b] + 1;
                pq.emplace(tmp, Dist[tmp]);
            }
            else if (Dist[tmp] > Dist[b] + 1)
            {
                LastStep[tmp] = b.get(nx, ny);
                Dist[tmp] = Dist[b] + 1;
                pq.emplace(tmp, Dist[tmp]);
            }
        }
    }
    return false;
}

int main()
{
#ifdef _WIN32
    system("chcp 65001");
#endif

    Board b, target;
    for (int i = 0; i < 16 - 1; ++i)
    {
        target.set(i / 4, i % 4, i + 1);
    }
    target.show();
    assert(target.predict() == 0 && target.isSolvable());

    bool flag = false;

    while (true) {
        b.clear();
        cout << "請輸入盤面的數字 (如上圖) 遇到空位請輸入 0 (0在最後可忽略) :" << endl;
        bool rZ = false;
        for (int i = 0; i < 16; ++i)
        {
            if (i == 15 && !rZ) break;

            ull v;
            cin >> v;
            if (v == 0) rZ = true;
            b.set(i / 4, i % 4, v);
        }

        if (!b.isSolvable())
        {
            cout << "你是不是輸入錯了 ?" << endl;
            b.show();
            continue;
        }

        if (b.predict() >= 35)
        {
            cout << "答案可能會找很久... 建議更換" << endl;
            cout << "預測最少步數 : " << b.predict() << endl;
            cout << "要加速  ? : " << endl;
            cout << "y : 加速 (可能答案好幾百步) " << endl;
            cout << "n : 不要加速 (我電腦超好) " << endl;
            cout << "r : 換下一個 " << endl;

            string s;
            cin >> s;
            if (tolower(s[0]) == 'y')
            {
                flag = true;
            }
            else if (tolower(s[0]) == 'n')
            {
                flag = false;
            }
            else
            {
                continue;
            }
        }
        break;
    }

    cout << "正在找答案... 參數(" << b.predict() << ',' << flag << ')' << endl;
    auto tbegin = clock();
    auto state = solve(b, target, flag);
    auto tend = clock();
    cout << "大概花了" << (tend-tbegin) / 1000.0 << "秒" << endl;
    if (!state)
    {
        cout << "有 BUG !? ，找不到答案" << endl;
    }
    else
    {
        vector<ull> res;

        Board current = target;
        while (LastStep[current] != 0)
        {
            ull mov = LastStep[current];
            res.emplace_back(mov);

            int mx, my, zx, zy;
            tie(mx, my) = current.loc(mov);
            tie(zx, zy) = current.loc(0);

            current.set(zx, zy, mov);
            current.set(mx, my, 0);
        }
        reverse(res.begin(), res.end());
        cout << "共" << res.size() << "步" << endl;
        cout << "請按順序點擊數字" << endl;

        int t = 1;
        for (auto s : res)
        {
            cout << "步驟 " << setw(2) << t++ << ": " << s << endl;
        }
    }
}
