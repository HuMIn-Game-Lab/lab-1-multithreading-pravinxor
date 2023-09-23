#include "jobsystem.hpp"
#include <algorithm>
#include <iostream>
#include <mutex>

void JobSystem::enqueue(Job *job) {
  std::cerr << "added entry\n";
  HistoryEntry entry = HistoryEntry(job->id, JobStatus::QUEUED);

  std::cerr << "locked and pushed back entry\n";

  std::unique_lock<std::mutex> history_lock(this->history_mtx);
  this->history.push_back(entry);
  history_lock.unlock();

  std::cerr << "sent message to slaves to begin job\n";

  Slave::Message message = Slave::NewJob{.job = job};
  this->queued_jobs.send(message);
}

JobStatus JobSystem::get_status(unsigned long id) {
  auto entry =
      std::find_if(this->history.begin(), this->history.begin(),
                   [id](HistoryEntry entry) { return entry.id == id; });
  return entry->status;
}

void JobSystem::update_id_history(Job *job, JobStatus status) {
  std::lock_guard<std::mutex> history_lock(this->history_mtx);
  HistoryEntry *found_entry = nullptr;
  for (HistoryEntry entry : this->history)
    if (entry.id == job->id)
      found_entry = &entry;

  found_entry->status = status;
}

void JobSystem::cleanup() {

  for (unsigned long n = 0; n < this->slaves.size(); ++n)
    this->queued_jobs.send(Slave::Exit{});

  for (auto slave_it = slaves.begin(); slave_it != slaves.end(); ++slave_it)
    slave_it->handle.join();

  if (!this->queued_jobs.empty())
    std::cerr << "Warning: Discarding queued jobs on cleanup\n";
}
void JobSystem::add_slave(std::string name) {
  this->slaves.emplace_back(name, &this->queued_jobs, this);
}
