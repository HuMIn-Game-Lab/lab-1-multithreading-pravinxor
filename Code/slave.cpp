#include "slave.hpp"
#include "jobsystem.hpp"

#include <iostream>
#include <thread>

Slave::Slave(std::string name, MessageQueue<Slave::Message> *job_waiting_mq,
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
      job->Execute();
      this->system->update_id_history(job, JobStatus::COMPLETED);
      job->JobCompleteCallback();
      this->system->update_id_history(job, JobStatus::RETIRED);
      break;

    case 1: // Stop the daemon
      return;

    default:
      std::cerr << "Error in slave: " << this->name
                << " Unhandled message type, ignoring\n";
    }
  }
}
