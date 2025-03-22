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
		executor_work_guard<io_context::executor_type> work_guard = make_work_guard(io_context); // io_context가 작업이 없어도 종료되지 않도록 작업 보장 (pool에 등록된 thread가 종료되지 않음)
		thread_pool pool(num_threads); // 스레드 풀 생성

		// 스레드 풀에서 io_context 실행
		for (unsigned int i = 0; i < num_threads; ++i)
		{
			post(pool, [&io_context]() { 
				io_context.run(); // 단일 io_context를 여러 스레드가 run()하면, 작업이 스레드 간에 자동으로 분배될 것
			});
		}

		// 서버 시작 (포트 12345)
		Server server(io_context, 12345, pool);

		pool.join(); // thread_pool이 종료될 때까지 대기
	}
	catch (exception& e) {
		cerr << "Exception: " << e.what() << "\n";
	}

	return 0;
}
