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
		thread_pool pool(num_threads); // ������ Ǯ ����

		// ���� ���� (��Ʈ 12345)
		Server server(io_context, 12345, pool);

		// I/O ���ؽ�Ʈ ����
		io_context.run();
	}
	catch (exception& e) {
		cerr << "Exception: " << e.what() << "\n";
	}
}
