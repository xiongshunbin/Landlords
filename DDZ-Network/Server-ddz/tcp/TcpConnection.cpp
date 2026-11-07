#include "TcpConnection.h"
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include "TcpServer.h"
#include "spdlog/spdlog.h"
#include "Codec.h"
#include "RsaCrypto.h"

TcpConnection::TcpConnection(int fd, EventLoop* evLoop) : m_evLoop(evLoop)
{
	m_readBuf = new Buffer(10240);
	m_writeBuf = new Buffer(10240);

	m_name = "Connection-" + std::to_string(fd);
	prepareSecretKey();
	m_channel = new Channel(fd, FDEvent::WriteEvent, processRead, processWrite, destroy, this);
	evLoop->addTask(m_channel, ElemType::ADD);
}

TcpConnection::~TcpConnection()
{
	if (m_readBuf != nullptr && m_readBuf->readableSize() == 0
		&& m_writeBuf != nullptr && m_writeBuf->readableSize() == 0)
	{
		delete m_readBuf;
		delete m_writeBuf;
		m_evLoop->freeChannel(m_channel);
	}
	spdlog::get("basic_logger")->info("连接断开, 释放资源, ConnectionName: {}", m_name);
}

void TcpConnection::prepareSecretKey()
{
	// 读取公钥
	std::ifstream ifs("public.pem");
	std::stringstream sstr;
	sstr << ifs.rdbuf();
	std::string data = sstr.str();

	// 发送数据
	Message msg;
	msg.rescode = ResponseCode::RsaFenFa;
	msg.data1 = data;
	RsaCrypto rsa("private.pem", RsaCrypto::PRIVATE_KEY);
	msg.data2 = rsa.sign(data);
	Codec codec(&msg);
	m_writeBuf->appendPackage(codec.encodeMessage());
}

int TcpConnection::processRead(void* arg)
{
	TcpConnection* conn = static_cast<TcpConnection*>(arg);
	// 接收数据
	int socketFd = conn->m_channel->getSocketFd();
	int count = conn->m_readBuf->socketRead(socketFd);
	spdlog::get("basic_logger")->info("服务器接收到的http请求数据: {}", conn->m_readBuf->data());

	if (count > 0)
	{
		// 解析客户端请求

	}
	else if (count <= 0)
	{
		// 断开客户端连接

	}

	return 0;
}

int TcpConnection::processWrite(void* arg)
{
	spdlog::get("basic_logger")->info("开始发送数据(基于写事件发送)......");
	TcpConnection* conn = static_cast<TcpConnection*>(arg);
	// 发送数据
	int count = conn->m_writeBuf->sendData(conn->m_channel->getSocketFd());
	if (count > 0)
	{
		// 判断数据是否被全部发送出去了
		if (conn->m_writeBuf->readableSize() == 0)
		{
			// 1.不再检测写事件 -- 修改 Channel 中保存的事件
			conn->m_channel->writeEventEnable(false);
			// 2.修改 Dispatcher 检查的集合 -- 添加任务节点
			conn->m_evLoop->addTask(conn->m_channel, ElemType::MODIFY);
		}
	}
	return 0;
}

int TcpConnection::destroy(void* arg)
{
	TcpConnection* conn = static_cast<TcpConnection*>(arg);
	if (conn != nullptr)
	{
		delete conn;
	}
	return 0;
}
