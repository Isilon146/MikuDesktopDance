#pragma once

// 終了用の関数
// 終了前に何かする事がある場合に利用できそうなのでとりあえず（\eとか）

class End{
private:
	static bool end;
public:
	static void Die();		// エラーがあった場合の終了
	static void SubEnd();	// 通常の終了
	static bool Get();		// 終了状態の取得
};