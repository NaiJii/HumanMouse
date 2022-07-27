#include "mouse.h"

int main() {
	std::atexit([] {std::cout << "EXITING"; Sleep(5000); });
	
	if (!HMouse::Setup())
		return 0;

	// print screenSize
	const HMouse::Vec2 screenSize = HMouse::detail::screenSize;
	std::cout << "screenSize: " << screenSize.x << "x" << screenSize.y << std::endl;
	
#if 0
	while (!GetAsyncKeyState(VK_NUMPAD0)) {
		HMouse::Move(10, 10);
		HMouse::Vec2 o = HMouse::GetMousePos();
		std::cout << "mousePos: " << o.x << "x" << o.y << std::endl;
		Sleep(200);
	}
#endif

#if 0
	// find a function that makes an inverted parabolla
	// that goes through (0,0), (w/2, h), (w, 0)
	double a, b, c;
	c = 0;
	b = 4 * (double)HMouse::detail::screenSize.y / (double)HMouse::detail::screenSize.x;
	a = -4 * (double)HMouse::detail::screenSize.y / (HMouse::detail::screenSize.x * HMouse::detail::screenSize.x);
	const auto f = [a, b, c](double x) { return a * x * x + b * x + c; };
	std::cout << "a: " << a << " b: " << b << " c: " << c << std::endl;
	
	for (double i = 0.; i < HMouse::detail::screenSize.x; i += 1.) {
		//std::cout << "i: " << i << " f(i): " << f(i) << std::endl;
		HMouse::Move<MoveType::MOVE_ABSOLUTE>(i, f(i));
		Sleep(2);
	}
#endif
	
#if 1
	HMouse::CreatePath();
	HMouse::MovePath();
#endif
	
	return 0;
}
