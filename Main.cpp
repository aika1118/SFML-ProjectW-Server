#include <thread>
#include <boost/asio.hpp>
#include "Session.h"
#include "Server.h"

int main()
{
	// �ý����� CPU �ھ� ���� ������
	unsigned int num_threads = thread::hardware_concurrency();

	// �ּ� 2���� ������ ����
	if (num_threads == 0) num_threads = 2;

	cout << "Number of threads: " << num_threads << endl;

	try {
		boost::asio::io_context io_context; // I/O ���ؽ�Ʈ ����
		executor_work_guard<io_context::executor_type> work_guard = make_work_guard(io_context); // io_context�� �۾��� ��� ������� �ʵ��� �۾� ���� (pool�� ��ϵ� thread�� ������� ����)
		thread_pool pool(num_threads); // ������ Ǯ ����

		// ������ Ǯ���� io_context ����
		for (unsigned int i = 0; i < num_threads; ++i)
		{
			post(pool, [&io_context]() { 
				io_context.run(); // ���� io_context�� ���� �����尡 run()�ϸ�, �۾��� ������ ���� �ڵ����� �й�� ��
			});
		}

		// ���� ���� (��Ʈ 12345)
		Server server(io_context, 12345, pool);

		pool.join(); // thread_pool�� ����� ������ ���
	}
	catch (exception& e) {
		cerr << "Exception: " << e.what() << "\n";
	}

	return 0;
}
