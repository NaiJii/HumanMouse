#pragma once

#include <Windows.h>

#include <vector>
#include <stack>
#include <random>
#include <iostream>

enum MoveType {
	MOVE_RELATIVE,
	MOVE_ABSOLUTE
};

namespace HMouse {
	struct Vec2 {
		int x, y;
	};

	namespace detail {
		inline Vec2 screenSize;
		inline std::vector<Vec2> path;
	}

	bool Setup() {
		// bruh
		::SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_SYSTEM_AWARE);
		detail::screenSize.x = GetSystemMetrics(SM_CXVIRTUALSCREEN);
		detail::screenSize.y = GetSystemMetrics(SM_CYVIRTUALSCREEN);
		
		if (detail::screenSize.x <= 0 || detail::screenSize.y <= 0) 
			return false;
		
		return true;
	}

	constexpr int Round(double x) {
		return x >= 0 ? int(x + 0.5) : int(x - 0.5);
	}

	// normalize position for mouse_event
	constexpr Vec2 Normalize(Vec2 pos) {
		double w = detail::screenSize.x;
		double h = detail::screenSize.y;
		return {
			Round((double)pos.x * 65535.0 / w) + 1,
			Round((double)pos.y * 65535.0 / h) + 1
		};
	}

	Vec2 GetMousePos() {
		POINT p;
		ZeroMemory( &p, sizeof(p) );
		GetCursorPos(&p);
		return { p.x, p.y };
	}

	template <MoveType M = MoveType::MOVE_RELATIVE>
	void Move(int dx, int dy) {
		Move<M>(Vec2(dx, dy));
	}

	template <MoveType M = MoveType::MOVE_RELATIVE>
	void Move(Vec2 move) {	
		if constexpr (M == MoveType::MOVE_RELATIVE) {
			const Vec2 mousePos = GetMousePos();
			move = Normalize(Vec2(mousePos.x + move.x, mousePos.y + move.y));
		}
		else if constexpr (M == MoveType::MOVE_ABSOLUTE) {
			move = Normalize(move);
		}

		mouse_event( MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE, move.x, move.y, 0, 0 );
	}

	void CreatePath() {
		while (!(GetAsyncKeyState(VK_LBUTTON) & 0x8000))
			Sleep(1);

		const Vec2 start = HMouse::GetMousePos();
		std::cout << "Start: " << start.x << ", " << start.y << std::endl;

		while(GetAsyncKeyState(VK_LBUTTON))
			Sleep(1);
		
		while (!(GetAsyncKeyState(VK_LBUTTON) & 0x8000))
			Sleep(1);

		const Vec2 end = HMouse::GetMousePos();
		std::cout << "End: " << end.x << ", " << end.y << std::endl;

		std::cout << "Enter number of points: ";
		int n = 0;
		std::cin >> n;
		if (n <= 0 || n > detail::screenSize.x) {
			std::cout << "Invalid number of points" << std::endl;
			return;
		}

		for (double i = 1; i < n; i++) {
			double x = start.x + (end.x - start.x) * i / n;
			double y = start.y + (end.y - start.y) * i / n;

			// Define random generator with Gaussian distribution
			const double mean = 0.0;
			const double stddev = 10.0;
			std::default_random_engine generator;
			std::normal_distribution<double> dist(mean, stddev);
			
			// Add Gaussian noise
			x += dist(generator);
			y += dist(generator);
			
			detail::path.push_back(Vec2(x, y));
		}
	}

	void MovePath() {
		for (Vec2 p : detail::path) {
			Move<MoveType::MOVE_ABSOLUTE>(p);
			Sleep(1);
		}
	}
}