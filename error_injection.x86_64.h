/* Here we design an error injection module for Uniserver project
 * This project is designed based on the QEMU version from UTH
 * In this header, we define the error generator API and QEMU injection API,
 * Error generator is used to generate random bit errors in a definited mem QEMU zone,
 * Change it to 64 bits
 * Written by Bin Wang (B.Wang@qub.ac.uk) 
 */  

#ifndef __ERROR_INJECTION_H__
#define __ERROR_INJECTION_H__

#include <unistd.h>
#include <sys/mman.h> 
#include <syslog.h>


#define ERROR_INJECTION_API  1

#define ERROR_SIMULATE_FILE   "/home/s3051296/error-report/error-simulate.txt"

#define ERROR_MEMRESULT_BIN   "/home/s3051296/error-report/mem-result.bin"

#define ERROR_REPORTS_FILE   "/home/s3051296/error-report/error-reports.txt"

#define CPU_NB_REGS32 8
#define CPU_NB_REGS64 16

// Simulation Mode
enum {
	ERROR_INJECTION_DISABLED,
	ERROR_INJECTION_GOLDEN_EXECUTION,
	ERROR_INJECTION_WITH_ERROR,
};

// Switch error injection mode on/off
enum {
	ERROR_INJECTION_DEBUG_OFF,
	ERROR_INJECTION_DEBUG_ON,
};

// Mode
enum {
	ERROR_INJECTION_MEM_CELL_ARRAY,
	// ERROR_INJECTION_MODULE_DECODER,
	ERROR_INJECTION_CPU_REGS,
	ERROR_INJECTION_MAX_TYPES,
};

// Mem Error Type Definiton 
enum {
	ERROR_INJECTION_STUCK_AT_ERROR,	
	ERROR_INJECTION_BITFLIP_ERROR,
	ERROR_INJECTION_COUPLING_ERROR,
	ERROR_INJECTION_MAX_MEM_ERROR_TYPES,
};

// CPU Error Type Definition
enum {
	ERROR_INJECTION_CPU_REG_STUCK_AT_ERROR,
	ERROR_INJECTION_CPU_REG_BITFLIP_ERROR,
//	ERROR_INJECTION_CPU_REG_PC_ERROR,
	ERROR_INJECTION_MAX_CPU_ERROR_TYPES,
};

// Decoder Error Type
enum {
	ERROR_INJECTION_FAILURE_TO_ACCESS_CELL,
	ERROR_INJECTION_WRONG_ACCESS_CELL,
	ERROR_INJECTION_ACCESS_MANY_CELLS_FOR_ONE_ADDRESS,
	ERROR_INJECTION_ACCESS_MANY_ADDRS_FOR_SAME_CELL,
};

// Trigger type
enum {
	ERROR_INJECTION_TIME_BASED_TRIGGER,
	ERROR_INJECTION_ACCESS_BASED_TRIGGER,
	ERROR_INJECTION_MAX_TRIGGER_TYPES,
};

// Duration
enum {
	ERROR_INJECTION_TRANSIENT,
	ERROR_INJECTION_PERMANENT,
	ERROR_INJECTION_INTERMITTENT,
	ERROR_INJECTION_MAX_DURATION,
};

struct ErrorGenerator {
	int mode;
	int mem_error_type;
	int decoder_error_type;
	int cpu_error_type;
	int trigger;
	int duration;

	uint64_t addr;
	int reg_idx;
	int bit_pos;
	int bit_val;
	int64_t start;
	int64_t end;

	// Used for the QEMU.git project from UTH

#define INVALID_VAL  (0x000000000)  // 64
#define VALID_VAL    (0x000000001)  // 64
	uint16_t flags; // 64
	uint8_t val[8]; 
};

struct ErrorInjectionStatistics {
//	uint8_t *pMemDump;  // change it to 64 bit
	uint8_t *pMemDump;
	unsigned long dataflow_err;
	unsigned long datapath_err;
};

struct ErrorInjectionModule {
	// Error injection mode
	int mode;
	// Configure debug mode
	int debug;

	// When the time is reach the system is restarted
	// end a new simulation is executed
	int64_t simultotalTime; 

	// The first simulation is to do a golden execution
	uint64_t goldenMemInit;
	uint64_t goldenMemEnd;
//	uint32_t goldenMemLen; //change it to 64 bit 
	uint32_t goldenMemLen;
	// Allocated memory with system expected results
//	uint8_t *pResults; //change it to 64 bit
	uint16_t *pResults;	
	int resultsLen;
//	uint8_t resultsIdxGoldenMem; // change it to 64 bit
	uint16_t resultsIdxGoldenMem;
	
	// Allocated memory with golden execution result
	// i386 arch -- the default arch definition, we can
	// also define the arch about X86_64, ARM and aarch64
//	uint8_t *goldenMemDump;  // change it to 64 bit
	uint8_t *goldenMemDump;	

	// Depends on the number of simulate in the input file
	int simtotal;
	int idx;
	struct ErrorGenerator *list;

	struct ErrorInjectionStatistics stats;
};

void cpu_reset_ticks(void);

int print_error_entry(void);

//int error_bin_init(void);

// uint32_t *error_benchmark_result(void); // change it to 64 bit
uint64_t *error_benchmark_result(void); // 64

int error_injection_init(void);

int error_injection_statistics(void);

void error_injection_export_results(void);

int error_injection_golden_sim(void);

int error_injection_mem_array(void);

int error_injection_decoder(void);

//int error_injection_cpu_regs(uint32_t *regs); // change it to 64 bit
int error_injection_cpu_regs(uint64_t *regs);

//int error_injection_check_and_trigger(uint32_t *regs); // change it to 64 bit
int error_injection_check_and_trigger(uint64_t *regs);

int error_injection_exception_handler(int exception_index, int error_code);

#endif

