// Local-only AP telemetry. No shared queues, policy, simulator time or RNG access.
#ifndef KOMONDOR_AP_OBSERVATION_H
#define KOMONDOR_AP_OBSERVATION_H
#include <cstdio>
#include <cstdlib>
#include <sstream>
#include <iomanip>
#include <string>
#include <stdexcept>
#include <limits>
#include <fcntl.h>
#include <unistd.h>

inline std::string ApJsonString(const std::string &value) {
    std::ostringstream out; out << (char)34;
    const char *hex="0123456789abcdef";
    for (std::string::size_type i=0;i<value.size();++i) {
        unsigned char c=(unsigned char)value[i];
        if (c==34 || c==92) out << (char)92 << (char)c;
        else if (c<32) out << (char)92 << "u00" << hex[c>>4] << hex[c&15];
        else out << (char)c;
    }
    out << (char)34; return out.str();
}
inline std::string ApJsonNumber(double value) {
    if (!(value==value) || value>std::numeric_limits<double>::max()
            || value<-std::numeric_limits<double>::max()) return "null";
    std::ostringstream out; out << std::setprecision(17) << value; return out.str();
}
inline const char *ApStateName(int state) {
    switch(state) {
        case STATE_DL_CFEND_HOLD: return "STATE_DL_CFEND_HOLD";
        case STATE_TX_CFEND: return "STATE_TX_CFEND";
        case STATE_UNKNOWN: return "STATE_UNKNOWN";
        case STATE_SENSING: return "STATE_SENSING";
        case STATE_TX_DATA: return "STATE_TX_DATA";
        case STATE_RX_DATA: return "STATE_RX_DATA";
        case STATE_WAIT_ACK: return "STATE_WAIT_ACK";
        case STATE_TX_ACK: return "STATE_TX_ACK";
        case STATE_RX_ACK: return "STATE_RX_ACK";
        case STATE_TX_RTS: return "STATE_TX_RTS";
        case STATE_TX_CTS: return "STATE_TX_CTS";
        case STATE_RX_RTS: return "STATE_RX_RTS";
        case STATE_RX_CTS: return "STATE_RX_CTS";
        case STATE_WAIT_CTS: return "STATE_WAIT_CTS";
        case STATE_WAIT_DATA: return "STATE_WAIT_DATA";
        case STATE_NAV: return "STATE_NAV";
        case STATE_SLEEP: return "STATE_SLEEP";
        case STATE_TX_ICF: return "STATE_TX_ICF";
        case STATE_WAIT_ICR: return "STATE_WAIT_ICR";
        case STATE_TX_TF: return "STATE_TX_TF";
        case STATE_RX_ICF: return "STATE_RX_ICF";
        case STATE_RX_ICR: return "STATE_RX_ICR";
        case STATE_TX_ICR: return "STATE_TX_ICR";
        case STATE_WAIT_MU_RTS: return "STATE_WAIT_MU_RTS";
        case STATE_RX_MU_RTS: return "STATE_RX_MU_RTS";
        case STATE_WAIT_TF: return "STATE_WAIT_TF";
        case STATE_RX_TF: return "STATE_RX_TF";
        case STATE_TX_ACK_TF: return "STATE_TX_ACK_TF";
        case STATE_WAIT_ACK_TF: return "STATE_WAIT_ACK_TF";
        case STATE_TX_DSO_ICF: return "STATE_TX_DSO_ICF";
        case STATE_RX_DSO_ICR: return "STATE_RX_DSO_ICR";
        case STATE_TX_DATA_DSO: return "STATE_TX_DATA_DSO";
        case STATE_WAIT_ACK_DSO: return "STATE_WAIT_ACK_DSO";
        case STATE_TX_NPCA_ICF: return "STATE_TX_NPCA_ICF";
        case STATE_RX_NPCA_ICR: return "STATE_RX_NPCA_ICR";
        case STATE_TX_DATA_NPCA: return "STATE_TX_DATA_NPCA";
        case STATE_WAIT_ACK_NPCA: return "STATE_WAIT_ACK_NPCA";
        case STATE_TX_NPCA_ICR: return "STATE_TX_NPCA_ICR";
        default: return "STATE_UNRECOGNIZED";
    }
}
struct ApObservation {
    FILE *stream;
    unsigned long sequence;
    int report_source;
    double report_time_s;
    bool report_has_data;
    bool allocation_valid;
    ApObservation() : stream(NULL), sequence(0), report_source(-1), report_time_s(-1), report_has_data(false), allocation_valid(false) {}
    ~ApObservation() { close(); }
    bool enabled() const { return stream!=NULL; }
    void open(int node_id) {
        const char *directory=std::getenv("KOMONDOR_AP_STATE_DIR");
        if (!directory) return;
        std::ostringstream path; path << directory << "/ap_" << node_id << ".jsonl";
        int fd=::open(path.str().c_str(),O_WRONLY|O_CREAT|O_EXCL,0666);
        if (fd<0) throw std::runtime_error("Cannot create independent AP state file: "+path.str());
        stream=fdopen(fd,"w");
        if (!stream) { ::close(fd); throw std::runtime_error("Cannot open AP state stream"); }
    }
    void write(const std::string &fields) {
        if (!stream) return;
        std::ostringstream line;
        line << "{\"schema\":\"komondor.ap_state.v1\",\"sequence\":" << sequence++ << ',' << fields << "}\n";
        std::string text=line.str();
        if (fwrite(text.data(),1,text.size(),stream)!=text.size() || fflush(stream)!=0)
            throw std::runtime_error("Cannot write AP state stream");
    }
    void close() { if (stream) { fclose(stream); stream=NULL; } }
};
#endif
