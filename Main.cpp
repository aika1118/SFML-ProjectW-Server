#include <thread>
#include <boost/asio.hpp>
#include "Session.h"
#include "Server.h"

int main()
{
	// 시스템의 CPU 코어 수를 가져옴
	unsigned int num_threads = thread::hardware_concurrency();

	// 최소 2개의 스레드 보장
	if (num_threads == 0) num_threads = 2;

	cout << "Number of threads: " << num_threads << endl;

	try {
		boost::asio::io_context io_context; // I/O 컨텍스트 생성
		thread_pool pool(num_threads); // 스레드 풀 생성

		// 서버 시작 (포트 12345)
		Server server(io_context, 12345, pool);

		// I/O 컨텍스트 실행
		io_context.run();
	}
	catch (exception& e) {
		cerr << "Exception: " << e.what() << "\n";
	}
}
