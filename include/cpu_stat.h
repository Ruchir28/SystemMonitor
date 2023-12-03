#ifndef CPU_STAT_H
#define CPU_STAT_H
#include <string>
class CpuStat {
    private:
        std::string cpu;
        float user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
    public:
        CpuStat() : cpu(""), user(0.0), nice(0.0), system(0.0), idle(0.0), iowait(0.0), irq(0.0), softirq(0.0), steal(0.0), guest(0.0), guest_nice(0.0) {}
        CpuStat(std::string cpu, float user, float nice, float system, float idle, float iowait, float irq, float softirq, float steal, float guest, float guest_nice) : cpu(cpu), user(user), nice(nice), system(system), idle(idle), iowait(iowait), irq(irq), softirq(softirq), steal(steal), guest(guest), guest_nice(guest_nice) {}
        std::string getCpu() const { return cpu; }
        float getUser() const { return user; }
        float getNice() const { return nice; }
        float getSystem() const { return system; }
        float getIdle() const { return idle; }
        float getIowait() const { return iowait; }
        float getIrq() const { return irq; }
        float getSoftirq() const { return softirq; }
        float getSteal() const { return steal; }
        float getGuest() const { return guest; }
        float getGuestNice() const { return guest_nice; }
        float getTotal() const { return user + nice + system + idle + iowait + irq + softirq + steal + guest + guest_nice; }
        float getIdleTotal() const { return idle + iowait; }
        float getNonIdleTotal() const { return user + nice + system + irq + softirq + steal + guest + guest_nice; }
        float operator-(CpuStat const& a) const { 
            float total_diff = getTotal() - a.getTotal();
            float idle_diff = getIdleTotal() - a.getIdleTotal();
            if(total_diff == 0.0) {
                return 0.0;
            }
            return (total_diff - idle_diff) / total_diff;
        }
};

#endif