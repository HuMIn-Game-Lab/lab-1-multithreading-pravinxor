#include "slave.hpp"
#include <iostream>

#include <thread>

Slave::Slave(std::string name, MessageQueue<SlaveMessage> *job_waiting_mq)
    : name(name), job_waiting_mq(job_waiting_mq) {
  this->handle = std::thread([this] { this->WorkDaemon(); });
}

void Slave::WorkDaemon() {
  SlaveMessage message;
  while (true) {
    message = this->job_waiting_mq->receive();

    Job *job;
    switch (message.index()) {
    case 0: // New Job
      job = std::get<NewJob>(message).job;
      job->Execute();
      break;

    default:
      std::cerr << "Error in slave: " << this->name
                << " Unhandled message type, exiting\n";
    case 1: // Stop the daemon
      return;
    }
  }
}
