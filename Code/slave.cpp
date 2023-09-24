#include "slave.hpp"
#include "jobsystem.hpp"

#include <iostream>
#include <thread>

Slave::Slave(std::string name,
             MessageQueue<Slave::Message, std::priority_queue,
                          Slave::CompareMessage> *job_waiting_mq,
             JobSystem *system)
    : name(name), system(system) {
  this->handle = std::thread([this] { this->WorkDaemon(); });
}

void Slave::WorkDaemon() {
  while (true) {
    Slave::Message message = this->system->queued_jobs.receive();

    Job *job;
    switch (message.index()) {
    case 0: // New Job
      job = std::get<NewJob>(message).job;

      this->system->update_id_history(job, JobStatus::RUNNING);
      job->execute();
      this->system->update_id_history(job, JobStatus::COMPLETED);

      job->chain_next(this->system);

      break;

    case 1: // Stop the daemon
      return;

    default: // Whoops?
      std::cerr << "Error in slave: " << this->name
                << " Unhandled message type, ignoring\n";
    }
  }
}
