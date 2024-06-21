
#if __COVERITY__
#if !defined(COVERITY_COMPAT_4a622ee35a6c29d94c1d6dbcfdfbe3b3)
#define COVERITY_COMPAT_4a622ee35a6c29d94c1d6dbcfdfbe3b3
#pragma builtin begin

/*
  Copyright (c) 2022 Synopsys, Inc. All rights reserved worldwide.
  The information contained in this file is the proprietary and confidential
  information of Synopsys, Inc. and its licensors, and is supplied subject to,
  and may be used only by Synopsys customers in accordance with the terms and
  conditions of a previously executed license agreement between Synopsys and
  that customer.
*/


/* DO NOT MODIFY THE CONTENTS OF THIS FILE */

// __coverity_cplusplus is provided to handle cases where the C++ standard
// __cplusplus predefined macro is not defined.
#define __coverity_cplusplus

#if defined(__APPLE__)
#if defined(__BLOCKS__)
#undef __BLOCKS__
#endif
const void* __builtin___CFStringMakeConstantString(const char*);
#endif

/* Copyright (c) 2023 Synopsys, Inc. All rights reserved worldwide. */
#ifdef __coverity_cplusplus
extern "C" {
#endif

#if defined(__arm__) || defined(__ARM_NEON__) || defined(__aarch64__)

// Type aliases common to several targets

typedef char __simd128_int8_t __attribute__((__vector_size__(16)));
typedef short __simd128_int16_t __attribute__((__vector_size__(16)));
typedef int __simd128_int32_t __attribute__((__vector_size__(16)));
typedef long long __simd128_int64_t __attribute__((__vector_size__(16)));
typedef unsigned char __simd128_uint8_t __attribute__((__vector_size__(16)));
typedef unsigned short __simd128_uint16_t __attribute__((__vector_size__(16)));
typedef unsigned int __simd128_uint32_t __attribute__((__vector_size__(16)));
typedef unsigned long long __simd128_uint64_t __attribute__((__vector_size__(16)));
typedef float cov_fp16_t __attribute__((mode(HF)));
typedef float __simd128_float32_t __attribute__((__vector_size__(16)));
typedef cov_fp16_t __simd128_float16_t __attribute__((__vector_size__(16)));
typedef int __builtin_neon_oi __attribute__ ((__vector_size__(32)));
typedef int __builtin_neon_xi __attribute__ ((__vector_size__(64)));
typedef cov_fp16_t __bf16;

#endif // defined(__arm__) || defined(__ARM_NEON__) || defined(__aarch64__)


#ifdef __arm__

void __builtin_cxa_end_cleanup();
void __builtin_arm_cdp(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int);
void __builtin_arm_cdp2(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int);
unsigned int __builtin_arm_crc32b(unsigned int, unsigned char);
unsigned int __builtin_arm_crc32cb(unsigned int, unsigned char);
unsigned int __builtin_arm_crc32ch(unsigned int, short unsigned int);
unsigned int __builtin_arm_crc32cw(unsigned int, unsigned int);
unsigned int __builtin_arm_crc32h(unsigned int, short unsigned int);
unsigned int __builtin_arm_crc32w(unsigned int, unsigned int);
void __builtin_arm_ldc(unsigned int, unsigned int, const void*);
void __builtin_arm_ldc2(unsigned int, unsigned int, const void*);
void __builtin_arm_ldc2l(unsigned int, unsigned int, const void*);
void __builtin_arm_ldcl(unsigned int, unsigned int, const void*);
void __builtin_arm_mcr(unsigned int, unsigned int, int, unsigned int, unsigned int, unsigned int);
void __builtin_arm_mcr2(unsigned int, unsigned int, int, unsigned int, unsigned int, unsigned int);
void __builtin_arm_mcrr(unsigned int, unsigned int, long long int, unsigned int);
void __builtin_arm_mcrr2(unsigned int, unsigned int, long long int, unsigned int);
int __builtin_arm_mrc(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int);
int __builtin_arm_mrc2(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int);
long long int __builtin_arm_mrrc(unsigned int, unsigned int, unsigned int);
long long int __builtin_arm_mrrc2(unsigned int, unsigned int, unsigned int);
void __builtin_arm_stc(unsigned int, unsigned int, void*);
void __builtin_arm_stc2(unsigned int, unsigned int, void*);
void __builtin_arm_stc2l(unsigned int, unsigned int, void*);
void __builtin_arm_stcl(unsigned int, unsigned int, void*);
void* __builtin___emutls_get_address(void*);
void __builtin___emutls_register_common(void*, unsigned int, unsigned int, void*);
unsigned int __builtin_arm_get_fpscr ();
void __builtin_arm_set_fpscr (unsigned int);

int __builtin_arm_saturation_occurred(void);
int __builtin_arm_qadd(int, int);
int __builtin_arm_qadd16(int, int);
int __builtin_arm_qadd8(int, int);
int __builtin_arm_qasx(int, int);
int __builtin_arm_qsax(int, int);
int __builtin_arm_qsub(int, int);
int __builtin_arm_qsub16(int, int);
int __builtin_arm_qsub8(int, int);
int __builtin_arm_sadd16(int, int);
int __builtin_arm_sadd8(int, int);
int __builtin_arm_sasx(int, int);
unsigned int __builtin_arm_sel(unsigned int, unsigned int);
void __builtin_arm_set_saturation(int);
int __builtin_arm_shadd16(int, int);
int __builtin_arm_shadd8(int, int);
int __builtin_arm_shasx(int, int);
int __builtin_arm_shsax(int, int);
int __builtin_arm_shsub16(int, int);
int __builtin_arm_shsub8(int, int);
int __builtin_arm_smlabb(int, int, int);
int __builtin_arm_smlad(int, int, int);
int __builtin_arm_smladx(int, int, int);
long long int __builtin_arm_smlald(int, int, long long int);
long long int __builtin_arm_smlaldx(int, int, long long int);
int __builtin_arm_smlatb(int, int, int);
int __builtin_arm_smlatt(int, int, int);
int __builtin_arm_smlawb(int, int, int);
int __builtin_arm_smlawt(int, int, int);
int __builtin_arm_smlsd(int, int, int);
int __builtin_arm_smlsdx(int, int, int);
long long int __builtin_arm_smlsld(int, int, long long int);
long long int __builtin_arm_smlsldx(int, int, long long int);
int __builtin_arm_smuad(int, int);
int __builtin_arm_smuadx(int, int);
int __builtin_arm_smusd(int, int);
int __builtin_arm_smusdx(int, int);
unsigned int __builtin_arm_ssat(int, unsigned int);
unsigned int __builtin_arm_ssat16(int, unsigned int);
int __builtin_arm_ssax(int, int);
int __builtin_arm_ssub16(int, int);
int __builtin_arm_ssub8(int, int);
int __builtin_arm_sxtab16(int, int);
int __builtin_arm_sxtb16(int);
unsigned int __builtin_arm_uadd16(unsigned int, unsigned int);
unsigned int __builtin_arm_uadd8(unsigned int, unsigned int);
unsigned int __builtin_arm_uasx(unsigned int, unsigned int);
unsigned int __builtin_arm_uhadd16(unsigned int, unsigned int);
unsigned int __builtin_arm_uhadd8(unsigned int, unsigned int);
unsigned int __builtin_arm_uhasx(unsigned int, unsigned int);
unsigned int __builtin_arm_uhsax(unsigned int, unsigned int);
unsigned int __builtin_arm_uhsub16(unsigned int, unsigned int);
unsigned int __builtin_arm_uhsub8(unsigned int, unsigned int);
unsigned int __builtin_arm_uqadd16(unsigned int, unsigned int);
unsigned int __builtin_arm_uqadd8(unsigned int, unsigned int);
unsigned int __builtin_arm_uqasx(unsigned int, unsigned int);
unsigned int __builtin_arm_uqsax(unsigned int, unsigned int);
unsigned int __builtin_arm_uqsub16(unsigned int, unsigned int);
unsigned int __builtin_arm_uqsub8(unsigned int, unsigned int);
unsigned int __builtin_arm_usad8(unsigned int, unsigned int);
unsigned int __builtin_arm_usada8(unsigned int, unsigned int, unsigned int);
unsigned int __builtin_arm_usat(int, unsigned int);
unsigned int __builtin_arm_usat16(int, unsigned int);
unsigned int __builtin_arm_usax(unsigned int, unsigned int);
unsigned int __builtin_arm_usub16(unsigned int, unsigned int);
unsigned int __builtin_arm_usub8(unsigned int, unsigned int);
unsigned int __builtin_arm_uxtab16(unsigned int, unsigned int);
unsigned int __builtin_arm_uxtb16(unsigned int);
void __builtin_sat_imm_check(int, int, int);
void* __emutls_get_address(void*);
void __emutls_register_common(void*, unsigned int, unsigned int, void*);
__bf16 __divbc3(__bf16, __bf16, __bf16, __bf16);
__bf16 __mulbc3(__bf16, __bf16, __bf16, __bf16);

// -mfp16-format=ieee
__bf16 __divhc3(__bf16, __bf16, __bf16, __bf16);
__bf16 __mulhc3(__bf16, __bf16, __bf16, __bf16);

// -march=armv8-m.main+cdecp0
long long int __builtin_arm_cx1adi(int, long long int, unsigned int);
int __builtin_arm_cx1asi(int, int, unsigned int);
long long int __builtin_arm_cx1di(int, unsigned int);
int __builtin_arm_cx1si(int, unsigned int);
long long int __builtin_arm_cx2adi(int, long long int, int, unsigned int);
int __builtin_arm_cx2asi(int, int, int, unsigned int);
long long int __builtin_arm_cx2di(int, int, unsigned int);
int __builtin_arm_cx2si(int, int, unsigned int);
long long int __builtin_arm_cx3adi(int, long long int, int, int, unsigned int);
int __builtin_arm_cx3asi(int, int, int, int, unsigned int);
long long int __builtin_arm_cx3di(int, int, int, unsigned int);
int __builtin_arm_cx3si(int, int, int, unsigned int);

// -mfloat-abi=hard -march=armv8.1-m.main+mve+cdecp0
long long int __builtin_arm_vcx1adi(int, long long int, unsigned int);
int __builtin_arm_vcx1asi(int, int, unsigned int);
long long int __builtin_arm_vcx1di(int, unsigned int);
__simd128_int8_t __builtin_arm_vcx1qa_p_v16qi(int, __simd128_int8_t, unsigned int, short unsigned int);
__simd128_int8_t __builtin_arm_vcx1qav16qi(int, __simd128_int8_t, unsigned int);
__simd128_int8_t __builtin_arm_vcx1q_p_v16qi(int, __simd128_int8_t, unsigned int, short unsigned int);
__simd128_int8_t __builtin_arm_vcx1qv16qi(int, unsigned int);
int __builtin_arm_vcx1si(int, unsigned int);
long long int __builtin_arm_vcx2adi(int, long long int, long long int, unsigned int);
int __builtin_arm_vcx2asi(int, int, int, unsigned int);
long long int __builtin_arm_vcx2di(int, long long int, unsigned int);
__simd128_int8_t __builtin_arm_vcx2qa_p_v16qi(int, __simd128_int8_t, __simd128_int8_t, unsigned int, short unsigned int);
__simd128_int8_t __builtin_arm_vcx2qav16qi(int, __simd128_int8_t, __simd128_int8_t, unsigned int);
__simd128_int8_t __builtin_arm_vcx2q_p_v16qi(int, __simd128_int8_t, __simd128_int8_t, unsigned int, short unsigned int);
__simd128_int8_t __builtin_arm_vcx2qv16qi(int, __simd128_int8_t, unsigned int);
int __builtin_arm_vcx2si(int, int, unsigned int);
long long int __builtin_arm_vcx3adi(int, long long int, long long int, long long int, unsigned int);
int __builtin_arm_vcx3asi(int, int, int, int, unsigned int);
long long int __builtin_arm_vcx3di(int, long long int, long long int, unsigned int);
__simd128_int8_t __builtin_arm_vcx3qa_p_v16qi(int, __simd128_int8_t, __simd128_int8_t, __simd128_int8_t, unsigned int, short unsigned int);
__simd128_int8_t __builtin_arm_vcx3qav16qi(int, __simd128_int8_t, __simd128_int8_t, __simd128_int8_t, unsigned int);
__simd128_int8_t __builtin_arm_vcx3q_p_v16qi(int, __simd128_int8_t, __simd128_int8_t, __simd128_int8_t, unsigned int, short unsigned int);
__simd128_int8_t __builtin_arm_vcx3qv16qi(int, __simd128_int8_t, __simd128_int8_t, unsigned int);
int __builtin_arm_vcx3si(int, int, int, unsigned int);

// -march=armv8.1-m.main+mve -mfloat-abi=hard
int __builtin_arm_get_fpscr_nzcvqc(void);
void __builtin_arm_set_fpscr_nzcvqc(int);
long long int __builtin_mve_sqrshrl_sat48_di(long long int, int);
long long int __builtin_mve_sqrshrl_sat64_di(long long int, int);
int __builtin_mve_sqrshr_si(int, int);
long long unsigned int __builtin_mve_sqshll_di(long long unsigned int, const int);
unsigned int __builtin_mve_sqshl_si(long long unsigned int, const int);
long long unsigned int __builtin_mve_srshrl_di(long long unsigned int, const int);
unsigned int __builtin_mve_srshr_si(long long unsigned int, const int);
long long unsigned int __builtin_mve_uqrshll_sat48_di(long long unsigned int, int);
long long unsigned int __builtin_mve_uqrshll_sat64_di(long long unsigned int, int);
unsigned int __builtin_mve_uqrshl_si(unsigned int, int);
long long unsigned int __builtin_mve_uqshll_di(long long unsigned int, const int);
unsigned int __builtin_mve_uqshl_si(unsigned int, const int);
long long unsigned int __builtin_mve_urshrl_di(long long unsigned int, const int);
unsigned int __builtin_mve_urshr_si(unsigned int, const int);
unsigned int __builtin_mve_vabavq_p_sv16qi(unsigned int, __simd128_int8_t, __simd128_int8_t, short unsigned int);
unsigned int __builtin_mve_vabavq_p_sv4si(unsigned int, __simd128_int32_t, __simd128_int32_t, short unsigned int);
unsigned int __builtin_mve_vabavq_p_sv8hi(unsigned int, __simd128_int16_t, __simd128_int16_t, short unsigned int);
unsigned int __builtin_mve_vabavq_p_uv16qi(unsigned int, __simd128_uint8_t, __simd128_uint8_t, short unsigned int);
unsigned int __builtin_mve_vabavq_p_uv4si(unsigned int, __simd128_uint32_t, __simd128_uint32_t, short unsigned int);
unsigned int __builtin_mve_vabavq_p_uv8hi(unsigned int, __simd128_uint16_t, __simd128_uint16_t, short unsigned int);
unsigned int __builtin_mve_vabavq_sv16qi(unsigned int, __simd128_int8_t, __simd128_int8_t);
unsigned int __builtin_mve_vabavq_sv4si(unsigned int, __simd128_int32_t, __simd128_int32_t);
unsigned int __builtin_mve_vabavq_sv8hi(unsigned int, __simd128_int16_t, __simd128_int16_t);
unsigned int __builtin_mve_vabavq_uv16qi(unsigned int, __simd128_uint8_t, __simd128_uint8_t);
unsigned int __builtin_mve_vabavq_uv4si(unsigned int, __simd128_uint32_t, __simd128_uint32_t);
unsigned int __builtin_mve_vabavq_uv8hi(unsigned int, __simd128_uint16_t, __simd128_uint16_t);
__simd128_float32_t __builtin_mve_vabdq_fv4sf(__simd128_float32_t, __simd128_float32_t);
__simd128_float16_t __builtin_mve_vabdq_fv8hf(__simd128_float16_t, __simd128_float16_t);
__simd128_float32_t __builtin_mve_vabdq_m_fv4sf(__simd128_float32_t, __simd128_float32_t, __simd128_float32_t, short unsigned int);
__simd128_float16_t __builtin_mve_vabdq_m_fv8hf(__simd128_float16_t, __simd128_float16_t, __simd128_float16_t, short unsigned int);
__simd128_int8_t __builtin_mve_vabdq_m_sv16qi(__simd128_int8_t, __simd128_int8_t, __simd128_int8_t, short unsigned int);
__simd128_int32_t __builtin_mve_vabdq_m_sv4si(__simd128_int32_t, __simd128_int32_t, __simd128_int32_t, short unsigned int);
__simd128_int16_t __builtin_mve_vabdq_m_sv8hi(__simd128_int16_t, __simd128_int16_t, __simd128_int16_t, short unsigned int);
__simd128_uint8_t __builtin_mve_vabdq_m_uv16qi(__simd128_uint8_t, __simd128_uint8_t, __simd128_uint8_t, short unsigned int);
__simd128_uint32_t __builtin_mve_vabdq_m_uv4si(__simd128_uint32_t, __simd128_uint32_t, __simd128_uint32_t, short unsigned int);
__simd128_uint16_t __builtin_mve_vabdq_m_uv8hi(__simd128_uint16_t, __simd128_uint16_t, __simd128_uint16_t, short unsigned int);
__simd128_int8_t __builtin_mve_vabdq_sv16qi(__simd128_int8_t, __simd128_int8_t);
__simd128_int32_t __builtin_mve_vabdq_sv4si(__simd128_int32_t, __simd128_int32_t);
__simd128_int16_t __builtin_mve_vabdq_sv8hi(__simd128_int16_t, __simd128_int16_t);
__simd128_uint8_t __builtin_mve_vabdq_uv16qi(__simd128_uint8_t, __simd128_uint8_t);
__simd128_uint32_t __builtin_mve_vabdq_uv4si(__simd128_uint32_t, __simd128_uint32_t);
__simd128_uint16_t __builtin_mve_vabdq_uv8hi(__simd128_uint16_t, __simd128_uint16_t);
__simd128_float32_t __builtin_mve_vabsq_fv4sf(__simd128_float32_t);
__simd128_float16_t __builtin_mve_vabsq_fv8hf(__simd128_float16_t);
__simd128_float32_t __builtin_mve_vabsq_m_fv4sf(__simd128_float32_t, __simd128_float32_t, short unsigned int);
__simd128_float16_t __builtin_mve_vabsq_m_fv8hf(__simd128_float16_t, __simd128_float16_t, short unsigned int);
__simd128_int8_t __builtin_mve_vabsq_m_sv16qi(__simd128_int8_t, __simd128_int8_t, short unsigned int);
__simd128_int32_t __builtin_mve_vabsq_m_sv4si(__simd128_int32_t, __simd128_int32_t, short unsigned int);
__simd128_int16_t __builtin_mve_vabsq_m_sv8hi(__simd128_int16_t, __simd128_int16_t, short unsigned int);
__simd128_int8_t __builtin_mve_vabsq_sv16qi(__simd128_int8_t);
__simd128_int32_t __builtin_mve_vabsq_sv4si(__simd128_int32_t);
__simd128_int16_t __builtin_mve_vabsq_sv8hi(__simd128_int16_t);
__simd128_int32_t __builtin_mve_vadciq_m_sv4si(__simd128_int32_t, __simd128_int32_t, __simd128_int32_t, short unsigned int);
__simd128_uint32_t __builtin_mve_vadciq_m_uv4si(__simd128_uint32_t, __simd128_uint32_t, __simd128_uint32_t, short unsigned int);
__simd128_int32_t __builtin_mve_vadciq_sv4si(__simd128_int32_t, __simd128_int32_t);
__simd128_uint32_t __builtin_mve_vadciq_uv4si(__simd128_uint32_t, __simd128_uint32_t);
__simd128_int32_t __builtin_mve_vadcq_m_sv4si(__simd128_int32_t, __simd128_int32_t, __simd128_int32_t, short unsigned int);
__simd128_uint32_t __builtin_mve_vadcq_m_uv4si(__simd128_uint32_t, __simd128_uint32_t, __simd128_uint32_t, short unsigned int);
__simd128_int32_t __builtin_mve_vadcq_sv4si(__simd128_int32_t, __simd128_int32_t);
__simd128_uint32_t __builtin_mve_vadcq_uv4si(__simd128_uint32_t, __simd128_uint32_t);
long long int __builtin_mve_vaddlvaq_p_sv4si(long long int, __simd128_int32_t, short unsigned int);
long long unsigned int __builtin_mve_vaddlvaq_p_uv4si(long long unsigned int, __simd128_uint32_t, short unsigned int);
long long int __builtin_mve_vaddlvaq_sv4si(long long int, __simd128_int32_t);
long long unsigned int __builtin_mve_vaddlvaq_uv4si(long long unsigned int, __simd128_uint32_t);
long long int __builtin_mve_vaddlvq_p_sv4si(__simd128_int32_t, short unsigned int);
long long unsigned int __builtin_mve_vaddlvq_p_uv4si(__simd128_uint32_t, short unsigned int);
long long int __builtin_mve_vaddlvq_sv4si(__simd128_int32_t);
long long unsigned int __builtin_mve_vaddlvq_uv4si(__simd128_uint32_t);
__simd128_float32_t __builtin_mve_vaddq_m_fv4sf(__simd128_float32_t, __simd128_float32_t, __simd128_float32_t, short unsigned int);
__simd128_float16_t __builtin_mve_vaddq_m_fv8hf(__simd128_float16_t, __simd128_float16_t, __simd128_float16_t, short unsigned int);
__simd128_float32_t __builtin_mve_vaddq_m_n_fv4sf(__simd128_float32_t, __simd128_float32_t, float, short unsigned int);
__simd128_float16_t __builtin_mve_vaddq_m_n_fv8hf(__simd128_float16_t, __simd128_float16_t, __bf16, short unsigned int);
__simd128_int8_t __builtin_mve_vaddq_m_n_sv16qi(__simd128_int8_t, __simd128_int8_t, signed char, short unsigned int);
__simd128_int32_t __builtin_mve_vaddq_m_n_sv4si(__simd128_int32_t, __simd128_int32_t, int, short unsigned int);
__simd128_int16_t __builtin_mve_vaddq_m_n_sv8hi(__simd128_int16_t, __simd128_int16_t, short int, short unsigned int);
__simd128_uint8_t __builtin_mve_vaddq_m_n_uv16qi(__simd128_uint8_t, __simd128_uint8_t, unsigned char, short unsigned int);
__simd128_uint32_t __builtin_mve_vaddq_m_n_uv4si(__simd128_uint32_t, __simd128_uint32_t, unsigned int, short unsigned int);
__simd128_uint16_t __builtin_mve_vaddq_m_n_uv8hi(__simd128_uint16_t, __simd128_uint16_t, short unsigned int, short unsigned int);
__simd128_int8_t __builtin_mve_vaddq_m_sv16qi(__simd128_int8_t, __simd128_int8_t, __simd128_int8_t, short unsigned int);
__simd128_int32_t __builtin_mve_vaddq_m_sv4si(__simd128_int32_t, __simd128_int32_t, __simd128_int32_t, short unsigned int);
__simd128_int16_t __builtin_mve_vaddq_m_sv8hi(__simd128_int16_t, __simd128_int16_t, __simd128_int16_t, short unsigned int);
__simd128_uint8_t __builtin_mve_vaddq_m_uv16qi(__simd128_uint8_t, __simd128_uint8_t, __simd128_uint8_t, short unsigned int);
__simd128_uint32_t __builtin_mve_vaddq_m_uv4si(__simd128_uint32_t, __simd128_uint32_t, __simd128_uint32_t, short unsigned int);
__simd128_uint16_t __builtin_mve_vaddq_m_uv8hi(__simd128_uint16_t, __simd128_uint16_t, __simd128_uint16_t, short unsigned int);
__simd128_float32_t __builtin_mve_vaddq_n_fv4sf(__simd128_float32_t, float);
__simd128_float16_t __builtin_mve_vaddq_n_fv8hf(__simd128_float16_t, __bf16);
__simd128_int8_t __builtin_mve_vaddq_n_sv16qi(__simd128_int8_t, signed char);
__simd128_int32_t __builtin_mve_vaddq_n_sv4si(__simd128_int32_t, int);
__simd128_int16_t __builtin_mve_vaddq_n_sv8hi(__simd128_int16_t, short int);
__simd128_uint8_t __builtin_mve_vaddq_n_uv16qi(__simd128_uint8_t, unsigned char);
__simd128_uint32_t __builtin_mve_vaddq_n_uv4si(__simd128_uint32_t, unsigned int);
__simd128_uint16_t __builtin_mve_vaddq_n_uv8hi(__simd128_uint16_t, short unsigned int);
int __builtin_mve_vaddvaq_p_sv16qi(int, __simd128_int8_t, short unsigned int);
int __builtin_mve_vaddvaq_p_sv4si(int, __simd128_int32_t, short unsigned int);
int __builtin_mve_vaddvaq_p_sv8hi(int, __simd128_int16_t, short unsigned int);
unsigned int __builtin_mve_vaddvaq_p_uv16qi(unsigned int, __simd128_uint8_t, short unsigned int);
unsigned int __builtin_mve_vaddvaq_p_uv4si(unsigned int, __simd128_uint32_t, short unsigned int);
unsigned int __builtin_mve_vaddvaq_p_uv8hi(unsigned int, __simd128_uint16_t, short unsigned int);
int __builtin_mve_vaddvaq_sv16qi(int, __simd128_int8_t);
int __builtin_mve_vaddvaq_sv4si(int, __simd128_int32_t);
int __builtin_mve_vaddvaq_sv8hi(int, __simd128_int16_t);
unsigned int __builtin_mve_vaddvaq_uv16qi(unsigned int, __simd128_uint8_t);
unsigned int __builtin_mve_vaddvaq_uv4si(unsigned int, __simd128_uint32_t);
unsigned int __builtin_mve_vaddvaq_uv8hi(unsigned int, __simd128_uint16_t);
int __builtin_mve_vaddvq_p_sv16qi(__simd128_int8_t, short unsigned int);
int __builtin_mve_vaddvq_p_sv4si(__simd128_int32_t, short unsigned int);
int __builtin_mve_vaddvq_p_sv8hi(__simd128_int16_t, short unsigned int);
unsigned int __builtin_mve_vaddvq_p_uv16qi(__simd128_uint8_t, short unsigned int);
unsigned int __builtin_mve_vaddvq_p_uv4si(__simd128_uint32_t, short unsigned int);
unsigned int __builtin_mve_vaddvq_p_uv8hi(__simd128_uint16_t, short unsigned int);
int __builtin_mve_vaddvq_sv16qi(__simd128_int8_t);
int __builtin_mve_vaddvq_sv4si(__simd128_int32_t);
int __builtin_mve_vaddvq_sv8hi(__simd128_int16_t);
unsigned int __builtin_mve_vaddvq_uv16qi(__simd128_uint8_t);
unsigned int __builtin_mve_vaddvq_uv4si(__simd128_uint32_t);
unsigned int __builtin_mve_vaddvq_uv8hi(__simd128_uint16_t);
__simd128_float32_t __builtin_mve_vandq_fv4sf(__simd128_float32_t, __simd128_float32_t);
__simd128_float16_t __builtin_mve_vandq_fv8hf(__simd128_float16_t, __simd128_float16_t);
__simd128_float32_t __builtin_mve_vandq_m_fv4sf(__simd128_float32_t, __simd128_float32_t, __simd128_float32_t, short unsigned int);
__simd128_float16_t __builtin_mve_vandq_m_fv8hf(__simd128_float16_t, __simd128_float16_t, __simd128_float16_t, short unsigned int);
__simd128_int8_t __builtin_mve_vandq_m_sv16qi(__simd128_int8_t, __simd128_int8_t, __simd128_int8_t, short unsigned int);
__simd128_int32_t __builtin_mve_vandq_m_sv4si(__simd128_int32_t, __simd128_int32_t, __simd128_int32_t, short unsigned int);
__simd128_int16_t __builtin_mve_vandq_m_sv8hi(__simd128_int16_t, __simd128_int16_t, __simd128_int16_t, short unsigned int);
__simd128_uint8_t __builtin_mve_vandq_m_uv16qi(__simd128_uint8_t, __simd128_uint8_t, __simd128_uint8_t, short unsigned int);
__simd128_uint32_t __builtin_mve_vandq_m_uv4si(__simd128_uint32_t, __simd128_uint32_t, __simd128_uint32_t, short unsigned int);
__simd128_uint16_t __builtin_mve_vandq_m_uv8hi(__simd128_uint16_t, __simd128_uint16_t, __simd128_uint16_t, short unsigned int);
__simd128_int8_t __builtin_mve_vandq_sv16qi(__simd128_int8_t, __simd128_int8_t);
__simd128_int32_t __builtin_mve_vandq_sv4si(__simd128_int32_t, __simd128_int32_t);
__simd128_int16_t __builtin_mve_vandq_sv8hi(__simd128_int16_t, __simd128_int16_t);
__simd128_uint8_t __builtin_mve_vandq_uv16qi(__simd128_uint8_t, __simd128_uint8_t);
__simd128_uint32_t __builtin_mve_vandq_uv4si(__simd128_uint32_t, __simd128_uint32_t);
__simd128_uint16_t __builtin_mve_vandq_uv8hi(__simd128_uint16_t, __simd128_uint16_t);
__simd128_float32_t __builtin_mve_vbicq_fv4sf(__simd128_float32_t, __simd128_float32_t);
__simd128_float16_t __builtin_mve_vbicq_fv8hf(__simd128_float16_t, __simd128_float16_t);
__simd128_float32_t __builtin_mve_vbicq_m_fv4sf(__simd128_float32_t, __simd128_float32_t, __simd128_float32_t, short unsigned int);
__simd128_float16_t __builtin_mve_vbicq_m_fv8hf(__simd128_float16_t, __simd128_float16_t, __simd128_float16_t, short unsigned int);
__simd128_int32_t __builtin_mve_vbicq_m_n_sv4si(__simd128_int32_t, int, short unsigned int);
__simd128_int16_t __builtin_mve_vbicq_m_n_sv8hi(__simd128_int16_t, int, short unsigned int);
__simd128_uint32_t __builtin_mve_vbicq_m_n_uv4si(__simd128_uint32_t, int, short unsigned int);
__simd128_uint16_t __builtin_mve_vbicq_m_n_uv8hi(__simd128_uint16_t, int, short unsigned int);
__simd128_int8_t __builtin_mve_vbicq_m_sv16qi(__simd128_int8_t, __simd128_int8_t, __simd128_int8_t, short unsigned int);
__simd128_int32_t __builtin_mve_vbicq_m_sv4si(__simd128_int32_t, __simd128_int32_t, __simd128_int32_t, short unsigned int);
__simd128_int16_t __builtin_mve_vbicq_m_sv8hi(__simd128_int16_t, __simd128_int16_t, __simd128_int16_t, short unsigned int);
__simd128_uint8_t __builtin_mve_vbicq_m_uv16qi(__simd128_uint8_t, __simd128_uint8_t, __simd128_uint8_t, short unsigned int);
__simd128_uint32_t __builtin_mve_vbicq_m_uv4si(__simd128_uint32_t, __simd128_uint32_t, __simd128_uint32_t, short unsigned int);
__simd128_uint16_t __builtin_mve_vbicq_m_uv8hi(__simd128_uint16_t, __simd128_uint16_t, __simd128_uint16_t, short unsigned int);
__simd128_int32_t __builtin_mve_vbicq_n_sv4si(__simd128_int32_t, int);
__simd128_int16_t __builtin_mve_vbicq_n_sv8hi(__simd128_int16_t, int);
__simd128_uint32_t __builtin_mve_vbicq_n_uv4si(__simd128_uint32_t, int);
__simd128_uint16_t __builtin_mve_vbicq_n_uv8hi(__simd128_uint16_t, int);
__simd128_int8_t __builtin_mve_vbicq_sv16qi(__simd128_int8_t, __simd128_int8_t);
__simd128_int32_t __builtin_mve_vbicq_sv4si(__simd128_int32_t, __simd128_int32_t);
__simd128_int16_t __builtin_mve_vbicq_sv8hi(__simd128_int16_t, __simd128_int16_t);
__simd128_uint8_t __builtin_mve_vbicq_uv16qi(__simd128_uint8_t, __simd128_uint8_t);
__simd128_uint32_t __builtin_mve_vbicq_uv4si(__simd128_uint32_t, __simd128_uint32_t);
__simd128_uint16_t __builtin_mve_vbicq_uv8hi(__simd128_uint16_t, __simd128_uint16_t);
__simd128_float32_t __builtin_mve_vbrsrq_m_n_fv4sf(__simd128_float32_t, __simd128_float32_t, int, short unsigned int);
__simd128_float16_t __builtin_mve_vbrsrq_m_n_fv8hf(__simd128_float16_t, __simd128_float16_t, int, short unsigned int);
__simd128_int8_t __builtin_mve_vbrsrq_m_n_sv16qi(__simd128_int8_t, __simd128_int8_t, int, short unsigned int);
__simd128_int32_t __builtin_mve_vbrsrq_m_n_sv4si(__simd128_int32_t, __simd128_int32_t, int, short unsigned int);
__simd128_int16_t __builtin_mve_vbrsrq_m_n_sv8hi(__simd128_int16_t, __simd128_int16_t, int, short unsigned int);
__simd128_uint8_t __builtin_mve_vbrsrq_m_n_uv16qi(__simd128_uint8_t, __simd128_uint8_t, int, short unsigned int);
__simd128_uint32_t __builtin_mve_vbrsrq_m_n_uv4si(__simd128_uint32_t, __simd128_uint32_t, int, short unsigned int);
__simd128_uint16_t __builtin_mve_vbrsrq_m_n_uv8hi(__simd128_uint16_t, __simd128_uint16_t, int, short unsigned int);
__simd128_float32_t __builtin_mve_vbrsrq_n_fv4sf(__simd128_float32_t, int);
__simd128_float16_t __builtin_mve_vbrsrq_n_fv8hf(__simd128_float16_t, int);
__simd128_int8_t __builtin_mve_vbrsrq_n_sv16qi(__simd128_int8_t, int);
__simd128_int32_t __builtin_mve_vbrsrq_n_sv4si(__simd128_int32_t, int);
__simd128_int16_t __builtin_mve_vbrsrq_n_sv8hi(__simd128_int16_t, int);
__simd128_uint8_t __builtin_mve_vbrsrq_n_uv16qi(__simd128_uint8_t, int);
__simd128_uint32_t __builtin_mve_vbrsrq_n_uv4si(__simd128_uint32_t, int);
__simd128_uint16_t __builtin_mve_vbrsrq_n_uv8hi(__simd128_uint16_t, int);
__simd128_float32_t __builtin_mve_vcaddq_rot270_fv4sf(__simd128_float32_t, __simd128_float32_t);
__simd128_float16_t __builtin_mve_vcaddq_rot270_fv8hf(__simd128_float16_t, __simd128_float16_t);
__simd128_float32_t __builtin_mve_vcaddq_rot270_m_fv4sf(__simd128_float32_t, __simd128_float32_t, __simd128_float32_t, short unsigned int);
__simd128_float16_t __builtin_mve_vcaddq_rot270_m_fv8hf(__simd128_float16_t, __simd128_float16_t, __simd128_float16_t, short unsigned int);
__simd128_int8_t __builtin_mve_vcaddq_rot270_m_sv16qi(__simd128_int8_t, __simd128_int8_t, __simd128_int8_t, short unsigned int);
__simd128_int32_t __builtin_mve_vcaddq_rot270_m_sv4si(__simd128_int32_t, __simd128_int32_t, __simd128_int32_t, short unsigned int);
__simd128_int16_t __builtin_mve_vcaddq_rot270_m_sv8hi(__simd128_int16_t, __simd128_int16_t, __simd128_int16_t, short unsigned int);
__simd128_uint8_t __builtin_mve_vcaddq_rot270_m_uv16qi(__simd128_uint8_t, __simd128_uint8_t, __simd128_uint8_t, short unsigned int);
__simd128_uint32_t __builtin_mve_vcaddq_rot270_m_uv4si(__simd128_uint32_t, __simd128_uint32_t, __simd128_uint32_t, short unsigned int);
__simd128_uint16_t __builtin_mve_vcaddq_rot270_m_uv8hi(__simd128_uint16_t, __simd128_uint16_t, __simd128_uint16_t, short unsigned int);
__simd128_int8_t __builtin_mve_vcaddq_rot270_sv16qi(__simd128_int8_t, __simd128_int8_t);
__simd128_int32_t __builtin_mve_vcaddq_rot270_sv4si(__simd128_int32_t, __simd128_int32_t);
__simd128_int16_t __builtin_mve_vcaddq_rot270_sv8hi(__simd128_int16_t, __simd128_int16_t);
__simd128_uint8_t __builtin_mve_vcaddq_rot270_uv16qi(__simd128_uint8_t, __simd128_uint8_t);
__simd128_uint32_t __builtin_mve_vcaddq_rot270_uv4si(__simd128_uint32_t, __simd128_uint32_t);
__simd128_uint16_t __builtin_mve_vcaddq_rot270_uv8hi(__simd128_uint16_t, __simd128_uint16_t);
__simd128_float32_t __builtin_mve_vcaddq_rot90_fv4sf(__simd128_float32_t, __simd128_float32_t);
__simd128_float16_t __builtin_mve_vcaddq_rot90_fv8hf(__simd128_float16_t, __simd128_float16_t);
__simd128_float32_t __builtin_mve_vcaddq_rot90_m_fv4sf(__simd128_float32_t, __simd128_float32_t, __simd128_float32_t, short unsigned int);
__simd128_float16_t __builtin_mve_vcaddq_rot90_m_fv8hf(__simd128_float16_t, __simd128_float16_t, __simd128_float16_t, short unsigned int);
__simd128_int8_t __builtin_mve_vcaddq_rot90_m_sv16qi(__simd128_int8_t, __simd128_int8_t, __simd128_int8_t, short unsigned int);
__simd128_int32_t __builtin_mve_vcaddq_rot90_m_sv4si(__simd128_int32_t, __simd128_int32_t, __simd128_int32_t, short unsigned int);
__simd128_int16_t __builtin_mve_vcaddq_rot90_m_sv8hi(__simd128_int16_t, __simd128_int16_t, __simd128_int16_t, short unsigned int);
__simd128_uint8_t __builtin_mve_vcaddq_rot90_m_uv16qi(__simd128_uint8_t, __simd128_uint8_t, __simd128_uint8_t, short unsigned int);
__simd128_uint32_t __builtin_mve_vcaddq_rot90_m_uv4si(__simd128_uint32_t, __simd128_uint32_t, __simd128_uint32_t, short unsigned int);
__simd128_uint16_t __builtin_mve_vcaddq_rot90_m_uv8hi(__simd128_uint16_t, __simd128_uint16_t, __simd128_uint16_t, short unsigned int);
__simd128_int8_t __builtin_mve_vcaddq_rot90_sv16qi(__simd128_int8_t, __simd128_int8_t);
__simd128_int32_t __builtin_mve_vcaddq_rot90_sv4si(__simd128_int32_t, __simd128_int32_t);
__simd128_int16_t __builtin_mve_vcaddq_rot90_sv8hi(__simd128_int16_t, __simd128_int16_t);
__simd128_uint8_t __builtin_mve_vcaddq_rot90_uv16qi(__simd128_uint8_t, __simd128_uint8_t);
__simd128_uint32_t __builtin_mve_vcaddq_rot90_uv4si(__simd128_uint32_t, __simd128_uint32_t);
__simd128_uint16_t __builtin_mve_vcaddq_rot90_uv8hi(__simd128_uint16_t, __simd128_uint16_t);
__simd128_int8_t __builtin_mve_vclsq_m_sv16qi(__simd128_int8_t, __simd128_int8_t, short unsigned int);
__simd128_int32_t __builtin_mve_vclsq_m_sv4si(__simd128_int32_t, __simd128_int32_t, short unsigned int);
__simd128_int16_t __builtin_mve_vclsq_m_sv8hi(__simd128_int16_t, __simd128_int16_t, short unsigned int);
__simd128_int8_t __builtin_mve_vclsq_sv16qi(__simd128_int8_t);
__simd128_int32_t __builtin_mve_vclsq_sv4si(__simd128_int32_t);
__simd128_int16_t __builtin_mve_vclsq_sv8hi(__simd128_int16_t);
__simd128_int8_t __builtin_mve_vclzq_m_sv16qi(__simd128_int8_t, __simd128_int8_t, short unsigned int);
__simd128_int32_t __builtin_mve_vclzq_m_sv4si(__simd128_int32_t, __simd128_int32_t, short unsigned int);
__simd128_int16_t __builtin_mve_vclzq_m_sv8hi(__simd128_int16_t, __simd128_int16_t, short unsigned int);
__simd128_uint8_t __builtin_mve_vclzq_m_uv16qi(__simd128_uint8_t, __simd128_uint8_t, short unsigned int);
__simd128_uint32_t __builtin_mve_vclzq_m_uv4si(__simd128_uint32_t, __simd128_uint32_t, short unsigned int);
__simd128_uint16_t __builtin_mve_vclzq_m_uv8hi(__simd128_uint16_t, __simd128_uint16_t, short unsigned int);
__simd128_int8_t __builtin_mve_vclzq_sv16qi(__simd128_int8_t);
__simd128_int32_t __builtin_mve_vclzq_sv4si(__simd128_int32_t);
__simd128_int16_t __builtin_mve_vclzq_sv8hi(__simd128_int16_t);
__simd128_uint8_t __builtin_mve_vclzq_uv16qi(__simd128_uint8_t);
__simd128_uint32_t __builtin_mve_vclzq_uv4si(__simd128_uint32_t);
__simd128_uint16_t __builtin_mve_vclzq_uv8hi(__simd128_uint16_t);
__simd128_float32_t __builtin_mve_vcmlaq_fv4sf(__simd128_float32_t, __simd128_float32_t, __simd128_float32_t);
__simd128_float16_t __builtin_mve_vcmlaq_fv8hf(__simd128_float16_t, __simd128_float16_t, __simd128_float16_t);
__simd128_float32_t __builtin_mve_vcmlaq_m_fv4sf(__simd128_float32_t, __simd128_float32_t, __simd128_float32_t, short unsigned int);
__simd128_float16_t __builtin_mve_vcmlaq_m_fv8hf(__simd128_float16_t, __simd128_float16_t, __simd128_float16_t, short unsigned int);
__simd128_float32_t __builtin_mve_vcmlaq_rot180_fv4sf(__simd128_float32_t, __simd128_float32_t, __simd128_float32_t);
__simd128_float16_t __builtin_mve_vcmlaq_rot180_fv8hf(__simd128_float16_t, __simd128_float16_t, __simd128_float16_t);
__simd128_float32_t __builtin_mve_vcmlaq_rot180_m_fv4sf(__simd128_float32_t, __simd128_float32_t, __simd128_float32_t, short unsigned int);
__simd128_float16_t __builtin_mve_vcmlaq_rot180_m_fv8hf(__simd128_float16_t, __simd128_float16_t, __simd128_float16_t, short unsigned int);
__simd128_float32_t __builtin_mve_vcmlaq_rot270_fv4sf(__simd128_float32_t, __simd128_float32_t, __simd128_float32_t);
__simd128_float16_t __builtin_mve_vcmlaq_rot270_fv8hf(__simd128_float16_t, __simd128_float16_t, __simd128_float16_t);
__simd128_float32_t __builtin_mve_vcmlaq_rot270_m_fv4sf(__simd128_float32_t, __simd128_float32_t, __simd128_float32_t, short unsigned int);
__simd128_float16_t __builtin_mve_vcmlaq_rot270_m_fv8hf(__simd128_float16_t, __simd128_float16_t, __simd128_float16_t, short unsigned int);
__simd128_float32_t __builtin_mve_vcmlaq_rot90_fv4sf(__simd128_float32_t, __simd128_float32_t, __simd128_float32_t);
__simd128_float16_t __builtin_mve_vcmlaq_rot90_fv8hf(__simd128_float16_t, __simd128_float16_t, __simd128_float16_t);
__simd128_float32_t __builtin_mve_vcmlaq_rot90_m_fv4sf(__simd128_float32_t, __simd128_float32_t, __simd128_float32_t, short unsigned int);
__simd128_float16_t __builtin_mve_vcmlaq_rot90_m_fv8hf(__simd128_float16_t, __simd128_float16_t, __simd128_float16_t, short unsigned int);
short unsigned int __builtin_mve_vcmpcsq_m_n_uv16qi(__simd128_uint8_t, unsigned char, short unsigned int);
short unsigned int __builtin_mve_vcmpcsq_m_n_uv4si(__simd128_uint32_t, unsigned int, short unsigned int);
short unsigned int __builtin_mve_vcmpcsq_m_n_uv8hi(__simd128_uint16_t, short unsigned int, short unsigned int);
short unsigned int __builtin_mve_vcmpcsq_m_uv16qi(__simd128_uint8_t, __simd128_uint8_t, short unsigned int);
short unsigned int __builtin_mve_vcmpcsq_m_uv4si(__simd128_uint32_t, __simd128_uint32_t, short unsigned int);
short unsigned int __builtin_mve_vcmpcsq_m_uv8hi(__simd128_uint16_t, __simd128_uint16_t, short unsigned int);
short unsigned int __builtin_mve_vcmpcsq_n_uv16qi(__simd128_uint8_t, unsigned char);
short unsigned int __builtin_mve_vcmpcsq_n_uv4si(__simd128_uint32_t, unsigned int);
short unsigned int __builtin_mve_vcmpcsq_n_uv8hi(__simd128_uint16_t, short unsigned int);
short unsigned int __builtin_mve_vcmpcsq_uv16qi(__simd128_uint8_t, __simd128_uint8_t);
short unsigned int __builtin_mve_vcmpcsq_uv4si(__simd128_uint32_t, __simd128_uint32_t);
short unsigned int __builtin_mve_vcmpcsq_uv8hi(__simd128_uint16_t, __simd128_uint16_t);
short unsigned int __builtin_mve_vcmpeqq_fv4sf(__simd128_float32_t, __simd128_float32_t);
short unsigned int __builtin_mve_vcmpeqq_fv8hf(__simd128_float16_t, __simd128_float16_t);
short unsigned int __builtin_mve_vcmpeqq_m_fv4sf(__simd128_float32_t, __simd128_float32_t, short unsigned int);
short unsigned int __builtin_mve_vcmpeqq_m_fv8hf(__simd128_float16_t, __simd128_float16_t, short unsigned int);
short unsigned int __builtin_mve_vcmpeqq_m_n_fv4sf(__simd128_float32_t, float, short unsigned int);
short unsigned int __builtin_mve_vcmpeqq_m_n_fv8hf(__simd128_float16_t, __bf16, short unsigned int);
short unsigned int __builtin_mve_vcmpeqq_m_n_sv16qi(__simd128_int8_t, signed char, short unsigned int);
short unsigned int __builtin_mve_vcmpeqq_m_n_sv4si(__simd128_int32_t, int, short unsigned int);
short unsigned int __builtin_mve_vcmpeqq_m_n_sv8hi(__simd128_int16_t, short int, short unsigned int);
short unsigned int __builtin_mve_vcmpeqq_m_n_uv16qi(__simd128_uint8_t, unsigned char, short unsigned int);
short unsigned int __builtin_mve_vcmpeqq_m_n_uv4si(__simd128_uint32_t, unsigned int, short unsigned int);
short unsigned int __builtin_mve_vcmpeqq_m_n_uv8hi(__simd128_uint16_t, short unsigned int, short unsigned int);
short unsigned int __builtin_mve_vcmpeqq_m_sv16qi(__simd128_int8_t, __simd128_int8_t, short unsigned int);
short unsigned int __builtin_mve_vcmpeqq_m_sv4si(__simd128_int32_t, __simd128_int32_t, short unsigned int);
short unsigned int __builtin_mve_vcmpeqq_m_sv8hi(__simd128_int16_t, __simd128_int16_t, short unsigned int);
short unsigned int __builtin_mve_vcmpeqq_m_uv16qi(__simd128_uint8_t, __simd128_uint8_t, short unsigned int);
short unsigned int __builtin_mve_vcmpeqq_m_uv4si(__simd128_uint32_t, __simd128_uint32_t, short unsigned int);
short unsigned int __builtin_mve_vcmpeqq_m_uv8hi(__simd128_uint16_t, __simd128_uint16_t, short unsigned int);
short unsigned int __builtin_mve_vcmpeqq_n_fv4sf(__simd128_float32_t, float);
short unsigned int __builtin_mve_vcmpeqq_n_fv8hf(__simd128_float16_t, __bf16);
short unsigned int __builtin_mve_vcmpeqq_n_sv16qi(__simd128_int8_t, signed char);
short unsigned int __builtin_mve_vcmpeqq_n_sv4si(__simd128_int32_t, int);
short unsigned int __builtin_mve_vcmpeqq_n_sv8hi(__simd128_int16_t, short int);
short unsigned int __builtin_mve_vcmpeqq_n_uv16qi(__simd128_uint8_t, unsigned char);
short unsigned int __builtin_mve_vcmpeqq_n_uv4si(__simd128_uint32_t, unsigned int);
short unsigned int __builtin_mve_vcmpeqq_n_uv8hi(__simd128_uint16_t, short unsigned int);
short unsigned int __builtin_mve_vcmpeqq_sv16qi(__simd128_int8_t, __simd128_int8_t);
short unsigned int __builtin_mve_vcmpeqq_sv4si(__simd128_int32_t, __simd128_int32_t);
short unsigned int __builtin_mve_vcmpeqq_sv8hi(__simd128_int16_t, __simd128_int16_t);
short unsigned int __builtin_mve_vcmpeqq_uv16qi(__simd128_uint8_t, __simd128_uint8_t);
short unsigned int __builtin_mve_vcmpeqq_uv4si(__simd128_uint32_t, __simd128_uint32_t);
short unsigned int __builtin_mve_vcmpeqq_uv8hi(__simd128_uint16_t, __simd128_uint16_t);
short unsigned int __builtin_mve_vcmpgeq_fv4sf(__simd128_float32_t, __simd128_float32_t);
short unsigned int __builtin_mve_vcmpgeq_fv8hf(__simd128_float16_t, __simd128_float16_t);
short unsigned int __builtin_mve_vcmpgeq_m_fv4sf(__simd128_float32_t, __simd128_float32_t, short unsigned int);
short unsigned int __builtin_mve_vcmpgeq_m_fv8hf(__simd128_float16_t, __simd128_float16_t, short unsigned int);
short unsigned int __builtin_mve_vcmpgeq_m_n_fv4sf(__simd128_float32_t, float, short unsigned int);
short unsigned int __builtin_mve_vcmpgeq_m_n_fv8hf(__simd128_float16_t, __bf16, short unsigned int);
short unsigned int __builtin_mve_vcmpgeq_m_n_sv16qi(__simd128_int8_t, signed char, short unsigned int);
short unsigned int __builtin_mve_vcmpgeq_m_n_sv4si(__simd128_int32_t, int, short unsigned int);
short unsigned int __builtin_mve_vcmpgeq_m_n_sv8hi(__simd128_int16_t, short int, short unsigned int);
short unsigned int __builtin_mve_vcmpgeq_m_sv16qi(__simd128_int8_t, __simd128_int8_t, short unsigned int);
short unsigned int __builtin_mve_vcmpgeq_m_sv4si(__simd128_int32_t, __simd128_int32_t, short unsigned int);
short unsigned int __builtin_mve_vcmpgeq_m_sv8hi(__simd128_int16_t, __simd128_int16_t, short unsigned int);
short unsigned int __builtin_mve_vcmpgeq_n_fv4sf(__simd128_float32_t, float);
short unsigned int __builtin_mve_vcmpgeq_n_fv8hf(__simd128_float16_t, __bf16);
short unsigned int __builtin_mve_vcmpgeq_n_sv16qi(__simd128_int8_t, signed char);
short unsigned int __builtin_mve_vcmpgeq_n_sv4si(__simd128_int32_t, int);
short unsigned int __builtin_mve_vcmpgeq_n_sv8hi(__simd128_int16_t, short int);
short unsigned int __builtin_mve_vcmpgeq_sv16qi(__simd128_int8_t, __simd128_int8_t);
short unsigned int __builtin_mve_vcmpgeq_sv4si(__simd128_int32_t, __simd128_int32_t);
short unsigned int __builtin_mve_vcmpgeq_sv8hi(__simd128_int16_t, __simd128_int16_t);
short unsigned int __builtin_mve_vcmpgtq_fv4sf(__simd128_float32_t, __simd128_float32_t);
short unsigned int __builtin_mve_vcmpgtq_fv8hf(__simd128_float16_t, __simd128_float16_t);
short unsigned int __builtin_mve_vcmpgtq_m_fv4sf(__simd128_float32_t, __simd128_float32_t, short unsigned int);
short unsigned int __builtin_mve_vcmpgtq_m_fv8hf(__simd128_float16_t, __simd128_float16_t, short unsigned int);
short unsigned int __builtin_mve_vcmpgtq_m_n_fv4sf(__simd128_float32_t, float, short unsigned int);
short unsigned int __builtin_mve_vcmpgtq_m_n_fv8hf(__simd128_float16_t, __bf16, short unsigned int);
short unsigned int __builtin_mve_vcmpgtq_m_n_sv16qi(__simd128_int8_t, signed char, short unsigned int);
short unsigned int __builtin_mve_vcmpgtq_m_n_sv4si(__simd128_int32_t, int, short unsigned int);
short unsigned int __builtin_mve_vcmpgtq_m_n_sv8hi(__simd128_int16_t, short int, short unsigned int);
short unsigned int __builtin_mve_vcmpgtq_m_sv16qi(__simd128_int8_t, __simd128_int8_t, short unsigned int);
short unsigned int __builtin_mve_vcmpgtq_m_sv4si(__simd128_int32_t, __simd128_int32_t, short unsigned int);
short unsigned int __builtin_mve_vcmpgtq_m_sv8hi(__simd128_int16_t, __simd128_int16_t, short unsigned int);
short unsigned int __builtin_mve_vcmpgtq_n_fv4sf(__simd128_float32_t, float);
short unsigned int __builtin_mve_vcmpgtq_n_fv8hf(__simd128_float16_t, __bf16);
short unsigned int __builtin_mve_vcmpgtq_n_sv16qi(__simd128_int8_t, signed char);
short unsigned int __builtin_mve_vcmpgtq_n_sv4si(__simd128_int32_t, int);
short unsigned int __builtin_mve_vcmpgtq_n_sv8hi(__simd128_int16_t, short int);
short unsigned int __builtin_mve_vcmpgtq_sv16qi(__simd128_int8_t, __simd128_int8_t);
short unsigned int __builtin_mve_vcmpgtq_sv4si(__simd128_int32_t, __simd128_int32_t);
short unsigned int __builtin_mve_vcmpgtq_sv8hi(__simd128_int16_t, __simd128_int16_t);
short unsigned int __builtin_mve_vcmphiq_m_n_uv16qi(__simd128_uint8_t, unsigned char, short unsigned int);
short unsigned int __builtin_mve_vcmphiq_m_n_uv4si(__simd128_uint32_t, unsigned int, short unsigned int);
short unsigned int __builtin_mve_vcmphiq_m_n_uv8hi(__simd128_uint16_t, short unsigned int, short unsigned int);
short unsigned int __builtin_mve_vcmphiq_m_uv16qi(__simd128_uint8_t, __simd128_uint8_t, short unsigned int);
short unsigned int __builtin_mve_vcmphiq_m_uv4si(__simd128_uint32_t, __simd128_uint32_t, short unsigned int);
short unsigned int __builtin_mve_vcmphiq_m_uv8hi(__simd128_uint16_t, __simd128_uint16_t, short unsigned int);
short unsigned int __builtin_mve_vcmphiq_n_uv16qi(__simd128_uint8_t, unsigned char);
short unsigned int __builtin_mve_vcmphiq_n_uv4si(__simd128_uint32_t, unsigned int);
short unsigned int __builtin_mve_vcmphiq_n_uv8hi(__simd128_uint16_t, short unsigned int);
short unsigned int __builtin_mve_vcmphiq_uv16qi(__simd128_uint8_t, __simd128_uint8_t);
short unsigned int __builtin_mve_vcmphiq_uv4si(__simd128_uint32_t, __simd128_uint32_t);
short unsigned int __builtin_mve_vcmphiq_uv8hi(__simd128_uint16_t, __simd128_uint16_t);
short unsigned int __builtin_mve_vcmpleq_fv4sf(__simd128_float32_t, __simd128_float32_t);
short unsigned int __builtin_mve_vcmpleq_fv8hf(__simd128_float16_t, __simd128_float16_t);
short unsigned int __builtin_mve_vcmpleq_m_fv4sf(__simd128_float32_t, __simd128_float32_t, short unsigned int);
short unsigned int __builtin_mve_vcmpleq_m_fv8hf(__simd128_float16_t, __simd128_float16_t, short unsigned int);
short unsigned int __builtin_mve_vcmpleq_m_n_fv4sf(__simd128_float32_t, float, short unsigned int);
short unsigned int __builtin_mve_vcmpleq_m_n_fv8hf(__simd128_float16_t, __bf16, short unsigned int);
short unsigned int __builtin_mve_vcmpleq_m_n_sv16qi(__simd128_int8_t, signed char, short unsigned int);
short unsigned int __builtin_mve_vcmpleq_m_n_sv4si(__simd128_int32_t, int, short unsigned int);
short unsigned int __builtin_mve_vcmpleq_m_n_sv8hi(__simd128_int16_t, short int, short unsigned int);
short unsigned int __builtin_mve_vcmpleq_m_sv16qi(__simd128_int8_t, __simd128_int8_t, short unsigned int);
short unsigned int __builtin_mve_vcmpleq_m_sv4si(__simd128_int32_t, __simd128_int32_t, short unsigned int);
short unsigned int __builtin_mve_vcmpleq_m_sv8hi(__simd128_int16_t, __simd128_int16_t, short unsigned int);
short unsigned int __builtin_mve_vcmpleq_n_fv4sf(__simd128_float32_t, float);
short unsigned int __builtin_mve_vcmpleq_n_fv8hf(__simd128_float16_t, __bf16);
short unsigned int __builtin_mve_vcmpleq_n_sv16qi(__simd128_int8_t, signed char);
short unsigned int __builtin_mve_vcmpleq_n_sv4si(__simd128_int32_t, int);
short unsigned int __builtin_mve_vcmpleq_n_sv8hi(__simd128_int16_t, short int);
short unsigned int __builtin_mve_vcmpleq_sv16qi(__simd128_int8_t, __simd128_int8_t);
short unsigned int __builtin_mve_vcmpleq_sv4si(__simd128_int32_t, __simd128_int32_t);
short unsigned int __builtin_mve_vcmpleq_sv8hi(__simd128_int16_t, __simd128_int16_t);
short unsigned int __builtin_mve_vcmpltq_fv4sf(__simd128_float32_t, __simd128_float32_t);
short unsigned int __builtin_mve_vcmpltq_fv8hf(__simd128_float16_t, __simd128_float16_t);
short unsigned int __builtin_mve_vcmpltq_m_fv4sf(__simd128_float32_t, __simd128_float32_t, short unsigned int);
short unsigned int __builtin_mve_vcmpltq_m_fv8hf(__simd128_float16_t, __simd128_float16_t, short unsigned int);
short unsigned int __builtin_mve_vcmpltq_m_n_fv4sf(__simd128_float32_t, float, short unsigned int);
short unsigned int __builtin_mve_vcmpltq_m_n_fv8hf(__simd128_float16_t, __bf16, short unsigned int);
short unsigned int __builtin_mve_vcmpltq_m_n_sv16qi(__simd128_int8_t, signed char, short unsigned int);
short unsigned int __builtin_mve_vcmpltq_m_n_sv4si(__simd128_int32_t, int, short unsigned int);
short unsigned int __builtin_mve_vcmpltq_m_n_sv8hi(__simd128_int16_t, short int, short unsigned int);
short unsigned int __builtin_mve_vcmpltq_m_sv16qi(__simd128_int8_t, __simd128_int8_t, short unsigned int);
short unsigned int __builtin_mve_vcmpltq_m_sv4si(__simd128_int32_t, __simd128_int32_t, short unsigned int);
short unsigned int __builtin_mve_vcmpltq_m_sv8hi(__simd128_int16_t, __simd128_int16_t, short unsigned int);
short unsigned int __builtin_mve_vcmpltq_n_fv4sf(__simd128_float32_t, float);
short unsigned int __builtin_mve_vcmpltq_n_fv8hf(__simd128_float16_t, __bf16);
short unsigned int __builtin_mve_vcmpltq_n_sv16qi(__simd128_int8_t, signed char);
short unsigned int __builtin_mve_vcmpltq_n_sv4si(__simd128_int32_t, int);
short unsigned int __builtin_mve_vcmpltq_n_sv8hi(__simd128_int16_t, short int);
short unsigned int __builtin_mve_vcmpltq_sv16qi(__simd128_int8_t, __simd128_int8_t);
short unsigned int __builtin_mve_vcmpltq_sv4si(__simd128_int32_t, __simd128_int32_t);
short unsigned int __builtin_mve_vcmpltq_sv8hi(__simd128_int16_t, __simd128_int16_t);
short unsigned int __builtin_mve_vcmpneq_fv4sf(__simd128_float32_t, __simd128_float32_t);
short unsigned int __builtin_mve_vcmpneq_fv8hf(__simd128_float16_t, __simd128_float16_t);
short unsigned int __builtin_mve_vcmpneq_m_fv4sf(__simd128_float32_t, __simd128_float32_t, short unsigned int);
short unsigned int __builtin_mve_vcmpneq_m_fv8hf(__simd128_float16_t, __simd128_float16_t, short unsigned int);
short unsigned int __builtin_mve_vcmpneq_m_n_fv4sf(__simd128_float32_t, float, short unsigned int);
short unsigned int __builtin_mve_vcmpneq_m_n_fv8hf(__simd128_float16_t, __bf16, short unsigned int);
short unsigned int __builtin_mve_vcmpneq_m_n_sv16qi(__simd128_int8_t, signed char, short unsigned int);
short unsigned int __builtin_mve_vcmpneq_m_n_sv4si(__simd128_int32_t, int, short unsigned int);
short unsigned int __builtin_mve_vcmpneq_m_n_sv8hi(__simd128_int16_t, short int, short unsigned int);
short unsigned int __builtin_mve_vcmpneq_m_n_uv16qi(__simd128_uint8_t, unsigned char, short unsigned int);
short unsigned int __builtin_mve_vcmpneq_m_n_uv4si(__simd128_uint32_t, unsigned int, short unsigned int);
short unsigned int __builtin_mve_vcmpneq_m_n_uv8hi(__simd128_uint16_t, short unsigned int, short unsigned int);
short unsigned int __builtin_mve_vcmpneq_m_sv16qi(__simd128_int8_t, __simd128_int8_t, short unsigned int);
short unsigned int __builtin_mve_vcmpneq_m_sv4si(__simd128_int32_t, __simd128_int32_t, short unsigned int);
short unsigned int __builtin_mve_vcmpneq_m_sv8hi(__simd128_int16_t, __simd128_int16_t, short unsigned int);
short unsigned int __builtin_mve_vcmpneq_m_uv16qi(__simd128_uint8_t, __simd128_uint8_t, short unsigned int);
short unsigned int __builtin_mve_vcmpneq_m_uv4si(__simd128_uint32_t, __simd128_uint32_t, short unsigned int);
short unsigned int __builtin_mve_vcmpneq_m_uv8hi(__simd128_uint16_t, __simd128_uint16_t, short unsigned int);
short unsigned int __builtin_mve_vcmpneq_n_fv4sf(__simd128_float32_t, float);
short unsigned int __builtin_mve_vcmpneq_n_fv8hf(__simd128_float16_t, __bf16);
short unsigned int __builtin_mve_vcmpneq_n_sv16qi(__simd128_int8_t, signed char);
short unsigned int __builtin_mve_vcmpneq_n_sv4si(__simd128_int32_t, int);
short unsigned int __builtin_mve_vcmpneq_n_sv8hi(__simd128_int16_t, short int);
short unsigned int __builtin_mve_vcmpneq_n_uv16qi(__simd128_uint8_t, unsigned char);
short unsigned int __builtin_mve_vcmpneq_n_uv4si(__simd128_uint32_t, unsigned int);
short unsigned int __builtin_mve_vcmpneq_n_uv8hi(__simd128_uint16_t, short unsigned int);
short unsigned int __builtin_mve_vcmpneq_sv16qi(__simd128_int8_t, __simd128_int8_t);
short unsigned int __builtin_mve_vcmpneq_sv4si(__simd128_int32_t, __simd128_int32_t);
short unsigned int __builtin_mve_vcmpneq_sv8hi(__simd128_int16_t, __simd128_int16_t);
short unsigned int __builtin_mve_vcmpneq_uv16qi(__simd128_uint8_t, __simd128_uint8_t);
short unsigned int __builtin_mve_vcmpneq_uv4si(__simd128_uint32_t, __simd128_uint32_t);
short unsigned int __builtin_mve_vcmpneq_uv8hi(__simd128_uint16_t, __simd128_uint16_t);
__simd128_float32_t __builtin_mve_vcmulq_fv4sf(__simd128_float32_t, __simd128_float32_t);
__simd128_float16_t __builtin_mve_vcmulq_fv8hf(__simd128_float16_t, __simd128_float16_t);
__simd128_float32_t __builtin_mve_vcmulq_m_fv4sf(__simd128_float32_t, __simd128_float32_t, __simd128_float32_t, short unsigned int);
__simd128_float16_t __builtin_mve_vcmulq_m_fv8hf(__simd128_float16_t, __simd128_float16_t, __simd128_float16_t, short unsigned int);
__simd128_float32_t __builtin_mve_vcmulq_rot180_fv4sf(__simd128_float32_t, __simd128_float32_t);
__simd128_float16_t __builtin_mve_vcmulq_rot180_fv8hf(__simd128_float16_t, __simd128_float16_t);
__simd128_float32_t __builtin_mve_vcmulq_rot180_m_fv4sf(__simd128_float32_t, __simd128_float32_t, __simd128_float32_t, short unsigned int);
__simd128_float16_t __builtin_mve_vcmulq_rot180_m_fv8hf(__simd128_float16_t, __simd128_float16_t, __simd128_float16_t, short unsigned int);
__simd128_float32_t __builtin_mve_vcmulq_rot270_fv4sf(__simd128_float32_t, __simd128_float32_t);
__simd128_float16_t __builtin_mve_vcmulq_rot270_fv8hf(__simd128_float16_t, __simd128_float16_t);
__simd128_float32_t __builtin_mve_vcmulq_rot270_m_fv4sf(__simd128_float32_t, __simd128_float32_t, __simd128_float32_t, short unsigned int);
__simd128_float16_t __builtin_mve_vcmulq_rot270_m_fv8hf(__simd128_float16_t, __simd128_float16_t, __simd128_float16_t, short unsigned int);
__simd128_float32_t __builtin_mve_vcmulq_rot90_fv4sf(__simd128_float32_t, __simd128_float32_t);
__simd128_float16_t __builtin_mve_vcmulq_rot90_fv8hf(__simd128_float16_t, __simd128_float16_t);
__simd128_float32_t __builtin_mve_vcmulq_rot90_m_fv4sf(__simd128_float32_t, __simd128_float32_t, __simd128_float32_t, short unsigned int);
__simd128_float16_t __builtin_mve_vcmulq_rot90_m_fv8hf(__simd128_float16_t, __simd128_float16_t, __simd128_float16_t, short unsigned int);
__simd128_float32_t __builtin_mve_vcreateq_fv4sf(long long unsigned int, long long unsigned int);
__simd128_float16_t __builtin_mve_vcreateq_fv8hf(long long unsigned int, long long unsigned int);
__simd128_int8_t __builtin_mve_vcreateq_sv16qi(long long unsigned int, long long unsigned int);
__simd128_int64_t __builtin_mve_vcreateq_sv2di(long long unsigned int, long long unsigned int);
__simd128_int32_t __builtin_mve_vcreateq_sv4si(long long unsigned int, long long unsigned int);
__simd128_int16_t __builtin_mve_vcreateq_sv8hi(long long unsigned int, long long unsigned int);
__simd128_uint8_t __builtin_mve_vcreateq_uv16qi(long long unsigned int, long long unsigned int);
__simd128_uint64_t __builtin_mve_vcreateq_uv2di(long long unsigned int, long long unsigned int);
__simd128_uint32_t __builtin_mve_vcreateq_uv4si(long long unsigned int, long long unsigned int);
__simd128_uint16_t __builtin_mve_vcreateq_uv8hi(long long unsigned int, long long unsigned int);
short unsigned int __builtin_mve_vctp16qhi(unsigned int);
short unsigned int __builtin_mve_vctp16q_mhi(unsigned int, short unsigned int);
short unsigned int __builtin_mve_vctp32qhi(unsigned int);
short unsigned int __builtin_mve_vctp32q_mhi(unsigned int, short unsigned int);
short unsigned int __builtin_mve_vctp64qhi(unsigned int);
short unsigned int __builtin_mve_vctp64q_mhi(unsigned int, short unsigned int);
short unsigned int __builtin_mve_vctp8qhi(unsigned int);
short unsigned int __builtin_mve_vctp8q_mhi(unsigned int, short unsigned int);
__simd128_int32_t __builtin_mve_vcvtaq_m_sv4si(__simd128_int32_t, __simd128_float32_t, short unsigned int);
__simd128_int16_t __builtin_mve_vcvtaq_m_sv8hi(__simd128_int16_t, __simd128_float16_t, short unsigned int);
__simd128_uint32_t __builtin_mve_vcvtaq_m_uv4si(__simd128_uint32_t, __simd128_float32_t, short unsigned int);
__simd128_uint16_t __builtin_mve_vcvtaq_m_uv8hi(__simd128_uint16_t, __simd128_float16_t, short unsigned int);
__simd128_int32_t __builtin_mve_vcvtaq_sv4si(__simd128_float32_t);
__simd128_int16_t __builtin_mve_vcvtaq_sv8hi(__simd128_float16_t);
__simd128_uint32_t __builtin_mve_vcvtaq_uv4si(__simd128_float32_t);
__simd128_uint16_t __builtin_mve_vcvtaq_uv8hi(__simd128_float16_t);
__simd128_float16_t __builtin_mve_vcvtbq_f16_f32v8hf(__simd128_float16_t, __simd128_float32_t);
__simd128_float32_t __builtin_mve_vcvtbq_f32_f16v4sf(__simd128_float16_t);
__simd128_float16_t __builtin_mve_vcvtbq_m_f16_f32v8hf(__simd128_float16_t, __simd128_float32_t, short unsigned int);
__simd128_float32_t __builtin_mve_vcvtbq_m_f32_f16v4sf(__simd128_float32_t, __simd128_float16_t, short unsigned int);
__simd128_int32_t __builtin_mve_vcvtmq_m_sv4si(__simd128_int32_t, __simd128_float32_t, short unsigned int);
__simd128_int16_t __builtin_mve_vcvtmq_m_sv8hi(__simd128_int16_t, __simd128_float16_t, short unsigned int);
__simd128_uint32_t __builtin_mve_vcvtmq_m_uv4si(__simd128_uint32_t, __simd128_float32_t, short unsigned int);
__simd128_uint16_t __builtin_mve_vcvtmq_m_uv8hi(__simd128_uint16_t, __simd128_float16_t, short unsigned int);
__simd128_int32_t __builtin_mve_vcvtmq_sv4si(__simd128_float32_t);
__simd128_int16_t __builtin_mve_vcvtmq_sv8hi(__simd128_float16_t);
__simd128_uint32_t __builtin_mve_vcvtmq_uv4si(__simd128_float32_t);
__simd128_uint16_t __builtin_mve_vcvtmq_uv8hi(__simd128_float16_t);
__simd128_int32_t __builtin_mve_vcvtnq_m_sv4si(__simd128_int32_t, __simd128_float32_t, short unsigned int);
__simd128_int16_t __builtin_mve_vcvtnq_m_sv8hi(__simd128_int16_t, __simd128_float16_t, short unsigned int);
__simd128_uint32_t __builtin_mve_vcvtnq_m_uv4si(__simd128_uint32_t, __simd128_float32_t, short unsigned int);
__simd128_uint16_t __builtin_mve_vcvtnq_m_uv8hi(__simd128_uint16_t, __simd128_float16_t, short unsigned int);
__simd128_int32_t __builtin_mve_vcvtnq_sv4si(__simd128_float32_t);
__simd128_int16_t __builtin_mve_vcvtnq_sv8hi(__simd128_float16_t);
__simd128_uint32_t __builtin_mve_vcvtnq_uv4si(__simd128_float32_t);
__simd128_uint16_t __builtin_mve_vcvtnq_uv8hi(__simd128_float16_t);
__simd128_int32_t __builtin_mve_vcvtpq_m_sv4si(__simd128_int32_t, __simd128_float32_t, short unsigned int);
__simd128_int16_t __builtin_mve_vcvtpq_m_sv8hi(__simd128_int16_t, __simd128_float16_t, short unsigned int);
__simd128_uint32_t __builtin_mve_vcvtpq_m_uv4si(__simd128_uint32_t, __simd128_float32_t, short unsigned int);
__simd128_uint16_t __builtin_mve_vcvtpq_m_uv8hi(__simd128_uint16_t, __simd128_float16_t, short unsigned int);
__simd128_int32_t __builtin_mve_vcvtpq_sv4si(__simd128_float32_t);
__simd128_int16_t __builtin_mve_vcvtpq_sv8hi(__simd128_float16_t);
__simd128_uint32_t __builtin_mve_vcvtpq_uv4si(__simd128_float32_t);
__simd128_uint16_t __builtin_mve_vcvtpq_uv8hi(__simd128_float16_t);
__simd128_int32_t __builtin_mve_vcvtq_from_f_sv4si(__simd128_float32_t);
__simd128_int16_t __builtin_mve_vcvtq_from_f_sv8hi(__simd128_float16_t);
__simd128_uint32_t __builtin_mve_vcvtq_from_f_uv4si(__simd128_float32_t);
__simd128_uint16_t __builtin_mve_vcvtq_from_f_uv8hi(__simd128_float16_t);
__simd128_int32_t __builtin_mve_vcvtq_m_from_f_sv4si(__simd128_int32_t, __simd128_float32_t, short unsigned int);
__simd128_int16_t __builtin_mve_vcvtq_m_from_f_sv8hi(__simd128_int16_t, __simd128_float16_t, short unsigned int);
__simd128_uint32_t __builtin_mve_vcvtq_m_from_f_uv4si(__simd128_uint32_t, __simd128_float32_t, short unsigned int);
__simd128_uint16_t __builtin_mve_vcvtq_m_from_f_uv8hi(__simd128_uint16_t, __simd128_float16_t, short unsigned int);
__simd128_int32_t __builtin_mve_vcvtq_m_n_from_f_sv4si(__simd128_int32_t, __simd128_float32_t, int, short unsigned int);
__simd128_int16_t __builtin_mve_vcvtq_m_n_from_f_sv8hi(__simd128_int16_t, __simd128_float16_t, int, short unsigned int);
__simd128_uint32_t __builtin_mve_vcvtq_m_n_from_f_uv4si(__simd128_uint32_t, __simd128_float32_t, int, short unsigned int);
__simd128_uint16_t __builtin_mve_vcvtq_m_n_from_f_uv8hi(__simd128_uint16_t, __simd128_float16_t, int, short unsigned int);
__simd128_float32_t __builtin_mve_vcvtq_m_n_to_f_sv4sf(__simd128_float32_t, __simd128_int32_t, int, short unsigned int);
__simd128_float16_t __builtin_mve_vcvtq_m_n_to_f_sv8hf(__simd128_float16_t, __simd128_int16_t, int, short unsigned int);
__simd128_float32_t __builtin_mve_vcvtq_m_n_to_f_uv4sf(__simd128_float32_t, __simd128_uint32_t, int, short unsigned int);
__simd128_float16_t __builtin_mve_vcvtq_m_n_to_f_uv8hf(__simd128_float16_t, __simd128_uint16_t, int, short unsigned int);
__simd128_float32_t __builtin_mve_vcvtq_m_to_f_sv4sf(__simd128_float32_t, __simd128_int32_t, short unsigned int);
__simd128_float16_t __builtin_mve_vcvtq_m_to_f_sv8hf(__simd128_float16_t, __simd128_int16_t, short unsigned int);
__simd128_float32_t __builtin_mve_vcvtq_m_to_f_uv4sf(__simd128_float32_t, __simd128_uint32_t, short unsigned int);
__simd128_float16_t __builtin_mve_vcvtq_m_to_f_uv8hf(__simd128_float16_t, __simd128_uint16_t, short unsigned int);
__simd128_int32_t __builtin_mve_vcvtq_n_from_f_sv4si(__simd128_float32_t, int);
__simd128_int16_t __builtin_mve_vcvtq_n_from_f_sv8hi(__simd128_float16_t, int);
__simd128_uint32_t __builtin_mve_vcvtq_n_from_f_uv4si(__simd128_float32_t, int);
__simd128_uint16_t __builtin_mve_vcvtq_n_from_f_uv8hi(__simd128_float16_t, int);
__simd128_float32_t __builtin_mve_vcvtq_n_to_f_sv4sf(__simd128_int32_t, int);
__simd128_float16_t __builtin_mve_vcvtq_n_to_f_sv8hf(__simd128_int16_t, int);
__simd128_float32_t __builtin_mve_vcvtq_n_to_f_uv4sf(__simd128_uint32_t, int);
__simd128_float16_t __builtin_mve_vcvtq_n_to_f_uv8hf(__simd128_uint16_t, int);
__simd128_float32_t __builtin_mve_vcvtq_to_f_sv4sf(__simd128_int32_t);
__simd128_float16_t __builtin_mve_vcvtq_to_f_sv8hf(__simd128_int16_t);
__simd128_float32_t __builtin_mve_vcvtq_to_f_uv4sf(__simd128_uint32_t);
__simd128_float16_t __builtin_mve_vcvtq_to_f_uv8hf(__simd128_uint16_t);
__simd128_float16_t __builtin_mve_vcvttq_f16_f32v8hf(__simd128_float16_t, __simd128_float32_t);
__simd128_float32_t __builtin_mve_vcvttq_f32_f16v4sf(__simd128_float16_t);
__simd128_float16_t __builtin_mve_vcvttq_m_f16_f32v8hf(__simd128_float16_t, __simd128_float32_t, short unsigned int);
__simd128_float32_t __builtin_mve_vcvttq_m_f32_f16v4sf(__simd128_float32_t, __simd128_float16_t, short unsigned int);
__simd128_uint8_t __builtin_mve_vddupq_m_n_uv16qi(__simd128_uint8_t, unsigned int, int, short unsigned int);
__simd128_uint32_t __builtin_mve_vddupq_m_n_uv4si(__simd128_uint32_t, unsigned int, int, short unsigned int);
__simd128_uint16_t __builtin_mve_vddupq_m_n_uv8hi(__simd128_uint16_t, unsigned int, int, short unsigned int);
__simd128_uint8_t __builtin_mve_vddupq_n_uv16qi(unsigned int, int);
__simd128_uint32_t __builtin_mve_vddupq_n_uv4si(unsigned int, int);
__simd128_uint16_t __builtin_mve_vddupq_n_uv8hi(unsigned int, int);
__simd128_float32_t __builtin_mve_vdupq_m_n_fv4sf(__simd128_float32_t, float, short unsigned int);
__simd128_float16_t __builtin_mve_vdupq_m_n_fv8hf(__simd128_float16_t, __bf16, short unsigned int);
__simd128_int8_t __builtin_mve_vdupq_m_n_sv16qi(__simd128_int8_t, signed char, short unsigned int);
__simd128_int32_t __builtin_mve_vdupq_m_n_sv4si(__simd128_int32_t, int, short unsigned int);
__simd128_int16_t __builtin_mve_vdupq_m_n_sv8hi(__simd128_int16_t, short int, short unsigned int);
__simd128_uint8_t __builtin_mve_vdupq_m_n_uv16qi(__simd128_uint8_t, unsigned char, short unsigned int);
__simd128_uint32_t __builtin_mve_vdupq_m_n_uv4si(__simd128_uint32_t, unsigned int, short unsigned int);
__simd128_uint16_t __builtin_mve_vdupq_m_n_uv8hi(__simd128_uint16_t, short unsigned int, short unsigned int);
__simd128_float32_t __builtin_mve_vdupq_n_fv4sf(float);
__simd128_float16_t __builtin_mve_vdupq_n_fv8hf(__bf16);
__simd128_int8_t __builtin_mve_vdupq_n_sv16qi(signed char);
__simd128_int32_t __builtin_mve_vdupq_n_sv4si(int);
__simd128_int16_t __builtin_mve_vdupq_n_sv8hi(short int);
__simd128_uint8_t __builtin_mve_vdupq_n_uv16qi(unsigned char);
__simd128_uint32_t __builtin_mve_vdupq_n_uv4si(unsigned int);
__simd128_uint16_t __builtin_mve_vdupq_n_uv8hi(short unsigned int);
__simd128_uint8_t __builtin_mve_vdwdupq_m_n_uv16qi(__simd128_uint8_t, unsigned int, long long unsigned int, int, short unsigned int);
__simd128_uint32_t __builtin_mve_vdwdupq_m_n_uv4si(__simd128_uint32_t, unsigned int, long long unsigned int, int, short unsigned int);
__simd128_uint16_t __builtin_mve_vdwdupq_m_n_uv8hi(__simd128_uint16_t, unsigned int, long long unsigned int, int, short unsigned int);
unsigned int __builtin_mve_vdwdupq_m_wb_uv16qi(__simd128_uint8_t, unsigned int, long long unsigned int, int, short unsigned int);
unsigned int __builtin_mve_vdwdupq_m_wb_uv4si(__simd128_uint32_t, unsigned int, long long unsigned int, int, short unsigned int);
unsigned int __builtin_mve_vdwdupq_m_wb_uv8hi(__simd128_uint16_t, unsigned int, long long unsigned int, int, short unsigned int);
__simd128_uint8_t __builtin_mve_vdwdupq_n_uv16qi(unsigned int, long long unsigned int, int);
__simd128_uint32_t __builtin_mve_vdwdupq_n_uv4si(unsigned int, long long unsigned int, int);
__simd128_uint16_t __builtin_mve_vdwdupq_n_uv8hi(unsigned int, long long unsigned int, int);
unsigned int __builtin_mve_vdwdupq_wb_uv16qi(unsigned int, long long unsigned int, int);
unsigned int __builtin_mve_vdwdupq_wb_uv4si(unsigned int, long long unsigned int, int);
unsigned int __builtin_mve_vdwdupq_wb_uv8hi(unsigned int, long long unsigned int, int);
__simd128_float32_t __builtin_mve_veorq_fv4sf(__simd128_float32_t, __simd128_float32_t);
__simd128_float16_t __builtin_mve_veorq_fv8hf(__simd128_float16_t, __simd128_float16_t);
__simd128_float32_t __builtin_mve_veorq_m_fv4sf(__simd128_float32_t, __simd128_float32_t, __simd128_float32_t, short unsigned int);
__simd128_float16_t __builtin_mve_veorq_m_fv8hf(__simd128_float16_t, __simd128_float16_t, __simd128_float16_t, short unsigned int);
__simd128_int8_t __builtin_mve_veorq_m_sv16qi(__simd128_int8_t, __simd128_int8_t, __simd128_int8_t, short unsigned int);
__simd128_int32_t __builtin_mve_veorq_m_sv4si(__simd128_int32_t, __simd128_int32_t, __simd128_int32_t, short unsigned int);
__simd128_int16_t __builtin_mve_veorq_m_sv8hi(__simd128_int16_t, __simd128_int16_t, __simd128_int16_t, short unsigned int);
__simd128_uint8_t __builtin_mve_veorq_m_uv16qi(__simd128_uint8_t, __simd128_uint8_t, __simd128_uint8_t, short unsigned int);
__simd128_uint32_t __builtin_mve_veorq_m_uv4si(__simd128_uint32_t, __simd128_uint32_t, __simd128_uint32_t, short unsigned int);
__simd128_uint16_t __builtin_mve_veorq_m_uv8hi(__simd128_uint16_t, __simd128_uint16_t, __simd128_uint16_t, short unsigned int);
__simd128_int8_t __builtin_mve_veorq_sv16qi(__simd128_int8_t, __simd128_int8_t);
__simd128_int32_t __builtin_mve_veorq_sv4si(__simd128_int32_t, __simd128_int32_t);
__simd128_int16_t __builtin_mve_veorq_sv8hi(__simd128_int16_t, __simd128_int16_t);
__simd128_uint8_t __builtin_mve_veorq_uv16qi(__simd128_uint8_t, __simd128_uint8_t);
__simd128_uint32_t __builtin_mve_veorq_uv4si(__simd128_uint32_t, __simd128_uint32_t);
__simd128_uint16_t __builtin_mve_veorq_uv8hi(__simd128_uint16_t, __simd128_uint16_t);
__simd128_float32_t __builtin_mve_vfmaq_fv4sf(__simd128_float32_t, __simd128_float32_t, __simd128_float32_t);
__simd128_float16_t __builtin_mve_vfmaq_fv8hf(__simd128_float16_t, __simd128_float16_t, __simd128_float16_t);
__simd128_float32_t __builtin_mve_vfmaq_m_fv4sf(__simd128_float32_t, __simd128_float32_t, __simd128_float32_t, short unsigned int);
__simd128_float16_t __builtin_mve_vfmaq_m_fv8hf(__simd128_float16_t, __simd128_float16_t, __simd128_float16_t, short unsigned int);
__simd128_float32_t __builtin_mve_vfmaq_m_n_fv4sf(__simd128_float32_t, __simd128_float32_t, float, short unsigned int);
__simd128_float16_t __builtin_mve_vfmaq_m_n_fv8hf(__simd128_float16_t, __simd128_float16_t, __bf16, short unsigned int);
__simd128_float32_t __builtin_mve_vfmaq_n_fv4sf(__simd128_float32_t, __simd128_float32_t, float);
__simd128_float16_t __builtin_mve_vfmaq_n_fv8hf(__simd128_float16_t, __simd128_float16_t, __bf16);
__simd128_float32_t __builtin_mve_vfmasq_m_n_fv4sf(__simd128_float32_t, __simd128_float32_t, float, short unsigned int);
__simd128_float16_t __builtin_mve_vfmasq_m_n_fv8hf(__simd128_float16_t, __simd128_float16_t, __bf16, short unsigned int);
__simd128_float32_t __builtin_mve_vfmasq_n_fv4sf(__simd128_float32_t, __simd128_float32_t, float);
__simd128_float16_t __builtin_mve_vfmasq_n_fv8hf(__simd128_float16_t, __simd128_float16_t, __bf16);
__simd128_float32_t __builtin_mve_vfmsq_fv4sf(__simd128_float32_t, __simd128_float32_t, __simd128_float32_t);
__simd128_float16_t __builtin_mve_vfmsq_fv8hf(__simd128_float16_t, __simd128_float16_t, __simd128_float16_t);
__simd128_float32_t __builtin_mve_vfmsq_m_fv4sf(__simd128_float32_t, __simd128_float32_t, __simd128_float32_t, short unsigned int);
__simd128_float16_t __builtin_mve_vfmsq_m_fv8hf(__simd128_float16_t, __simd128_float16_t, __simd128_float16_t, short unsigned int);
__simd128_int8_t __builtin_mve_vhaddq_m_n_sv16qi(__simd128_int8_t, __simd128_int8_t, signed char, short unsigned int);
__simd128_int32_t __builtin_mve_vhaddq_m_n_sv4si(__simd128_int32_t, __simd128_int32_t, int, short unsigned int);
__simd128_int16_t __builtin_mve_vhaddq_m_n_sv8hi(__simd128_int16_t, __simd128_int16_t, short int, short unsigned int);
__simd128_uint8_t __builtin_mve_vhaddq_m_n_uv16qi(__simd128_uint8_t, __simd128_uint8_t, unsigned char, short unsigned int);
__simd128_uint32_t __builtin_mve_vhaddq_m_n_uv4si(__simd128_uint32_t, __simd128_uint32_t, unsigned int, short unsigned int);
__simd128_uint16_t __builtin_mve_vhaddq_m_n_uv8hi(__simd128_uint16_t, __simd128_uint16_t, short unsigned int, short unsigned int);
__simd128_int8_t __builtin_mve_vhaddq_m_sv16qi(__simd128_int8_t, __simd128_int8_t, __simd128_int8_t, short unsigned int);
__simd128_int32_t __builtin_mve_vhaddq_m_sv4si(__simd128_int32_t, __simd128_int32_t, __simd128_int32_t, short unsigned int);
__simd128_int16_t __builtin_mve_vhaddq_m_sv8hi(__simd128_int16_t, __simd128_int16_t, __simd128_int16_t, short unsigned int);
__simd128_uint8_t __builtin_mve_vhaddq_m_uv16qi(__simd128_uint8_t, __simd128_uint8_t, __simd128_uint8_t, short unsigned int);
__simd128_uint32_t __builtin_mve_vhaddq_m_uv4si(__simd128_uint32_t, __simd128_uint32_t, __simd128_uint32_t, short unsigned int);
__simd128_uint16_t __builtin_mve_vhaddq_m_uv8hi(__simd128_uint16_t, __simd128_uint16_t, __simd128_uint16_t, short unsigned int);
__simd128_int8_t __builtin_mve_vhaddq_n_sv16qi(__simd128_int8_t, signed char);
__simd128_int32_t __builtin_mve_vhaddq_n_sv4si(__simd128_int32_t, int);
__simd128_int16_t __builtin_mve_vhaddq_n_sv8hi(__simd128_int16_t, short int);
__simd128_uint8_t __builtin_mve_vhaddq_n_uv16qi(__simd128_uint8_t, unsigned char);
__simd128_uint32_t __builtin_mve_vhaddq_n_uv4si(__simd128_uint32_t, unsigned int);
__simd128_uint16_t __builtin_mve_vhaddq_n_uv8hi(__simd128_uint16_t, short unsigned int);
__simd128_int8_t __builtin_mve_vhaddq_sv16qi(__simd128_int8_t, __simd128_int8_t);
__simd128_int32_t __builtin_mve_vhaddq_sv4si(__simd128_int32_t, __simd128_int32_t);
__simd128_int16_t __builtin_mve_vhaddq_sv8hi(__simd128_int16_t, __simd128_int16_t);
__simd128_uint8_t __builtin_mve_vhaddq_uv16qi(__simd128_uint8_t, __simd128_uint8_t);
__simd128_uint32_t __builtin_mve_vhaddq_uv4si(__simd128_uint32_t, __simd128_uint32_t);
__simd128_uint16_t __builtin_mve_vhaddq_uv8hi(__simd128_uint16_t, __simd128_uint16_t);
__simd128_int8_t __builtin_mve_vhcaddq_rot270_m_sv16qi(__simd128_int8_t, __simd128_int8_t, __simd128_int8_t, short unsigned int);
__simd128_int32_t __builtin_mve_vhcaddq_rot270_m_sv4si(__simd128_int32_t, __simd128_int32_t, __simd128_int32_t, short unsigned int);
__simd128_int16_t __builtin_mve_vhcaddq_rot270_m_sv8hi(__simd128_int16_t, __simd128_int16_t, __simd128_int16_t, short unsigned int);
__simd128_int8_t __builtin_mve_vhcaddq_rot270_sv16qi(__simd128_int8_t, __simd128_int8_t);
__simd128_int32_t __builtin_mve_vhcaddq_rot270_sv4si(__simd128_int32_t, __simd128_int32_t);
__simd128_int16_t __builtin_mve_vhcaddq_rot270_sv8hi(__simd128_int16_t, __simd128_int16_t);
__simd128_int8_t __builtin_mve_vhcaddq_rot90_m_sv16qi(__simd128_int8_t, __simd128_int8_t, __simd128_int8_t, short unsigned int);
__simd128_int32_t __builtin_mve_vhcaddq_rot90_m_sv4si(__simd128_int32_t, __simd128_int32_t, __simd128_int32_t, short unsigned int);
__simd128_int16_t __builtin_mve_vhcaddq_rot90_m_sv8hi(__simd128_int16_t, __simd128_int16_t, __simd128_int16_t, short unsigned int);
__simd128_int8_t __builtin_mve_vhcaddq_rot90_sv16qi(__simd128_int8_t, __simd128_int8_t);
__simd128_int32_t __builtin_mve_vhcaddq_rot90_sv4si(__simd128_int32_t, __simd128_int32_t);
__simd128_int16_t __builtin_mve_vhcaddq_rot90_sv8hi(__simd128_int16_t, __simd128_int16_t);
__simd128_int8_t __builtin_mve_vhsubq_m_n_sv16qi(__simd128_int8_t, __simd128_int8_t, signed char, short unsigned int);
__simd128_int32_t __builtin_mve_vhsubq_m_n_sv4si(__simd128_int32_t, __simd128_int32_t, int, short unsigned int);
__simd128_int16_t __builtin_mve_vhsubq_m_n_sv8hi(__simd128_int16_t, __simd128_int16_t, short int, short unsigned int);
__simd128_uint8_t __builtin_mve_vhsubq_m_n_uv16qi(__simd128_uint8_t, __simd128_uint8_t, unsigned char, short unsigned int);
__simd128_uint32_t __builtin_mve_vhsubq_m_n_uv4si(__simd128_uint32_t, __simd128_uint32_t, unsigned int, short unsigned int);
__simd128_uint16_t __builtin_mve_vhsubq_m_n_uv8hi(__simd128_uint16_t, __simd128_uint16_t, short unsigned int, short unsigned int);
__simd128_int8_t __builtin_mve_vhsubq_m_sv16qi(__simd128_int8_t, __simd128_int8_t, __simd128_int8_t, short unsigned int);
__simd128_int32_t __builtin_mve_vhsubq_m_sv4si(__simd128_int32_t, __simd128_int32_t, __simd128_int32_t, short unsigned int);
__simd128_int16_t __builtin_mve_vhsubq_m_sv8hi(__simd128_int16_t, __simd128_int16_t, __simd128_int16_t, short unsigned int);
__simd128_uint8_t __builtin_mve_vhsubq_m_uv16qi(__simd128_uint8_t, __simd128_uint8_t, __simd128_uint8_t, short unsigned int);
__simd128_uint32_t __builtin_mve_vhsubq_m_uv4si(__simd128_uint32_t, __simd128_uint32_t, __simd128_uint32_t, short unsigned int);
__simd128_uint16_t __builtin_mve_vhsubq_m_uv8hi(__simd128_uint16_t, __simd128_uint16_t, __simd128_uint16_t, short unsigned int);
__simd128_int8_t __builtin_mve_vhsubq_n_sv16qi(__simd128_int8_t, signed char);
__simd128_int32_t __builtin_mve_vhsubq_n_sv4si(__simd128_int32_t, int);
__simd128_int16_t __builtin_mve_vhsubq_n_sv8hi(__simd128_int16_t, short int);
__simd128_uint8_t __builtin_mve_vhsubq_n_uv16qi(__simd128_uint8_t, unsigned char);
__simd128_uint32_t __builtin_mve_vhsubq_n_uv4si(__simd128_uint32_t, unsigned int);
__simd128_uint16_t __builtin_mve_vhsubq_n_uv8hi(__simd128_uint16_t, short unsigned int);
__simd128_int8_t __builtin_mve_vhsubq_sv16qi(__simd128_int8_t, __simd128_int8_t);
__simd128_int32_t __builtin_mve_vhsubq_sv4si(__simd128_int32_t, __simd128_int32_t);
__simd128_int16_t __builtin_mve_vhsubq_sv8hi(__simd128_int16_t, __simd128_int16_t);
__simd128_uint8_t __builtin_mve_vhsubq_uv16qi(__simd128_uint8_t, __simd128_uint8_t);
__simd128_uint32_t __builtin_mve_vhsubq_uv4si(__simd128_uint32_t, __simd128_uint32_t);
__simd128_uint16_t __builtin_mve_vhsubq_uv8hi(__simd128_uint16_t, __simd128_uint16_t);
__simd128_uint8_t __builtin_mve_vidupq_m_n_uv16qi(__simd128_uint8_t, unsigned int, int, short unsigned int);
__simd128_uint32_t __builtin_mve_vidupq_m_n_uv4si(__simd128_uint32_t, unsigned int, int, short unsigned int);
__simd128_uint16_t __builtin_mve_vidupq_m_n_uv8hi(__simd128_uint16_t, unsigned int, int, short unsigned int);
__simd128_uint8_t __builtin_mve_vidupq_n_uv16qi(unsigned int, int);
__simd128_uint32_t __builtin_mve_vidupq_n_uv4si(unsigned int, int);
__simd128_uint16_t __builtin_mve_vidupq_n_uv8hi(unsigned int, int);
__simd128_uint8_t __builtin_mve_viwdupq_m_n_uv16qi(__simd128_uint8_t, unsigned int, long long unsigned int, int, short unsigned int);
__simd128_uint32_t __builtin_mve_viwdupq_m_n_uv4si(__simd128_uint32_t, unsigned int, long long unsigned int, int, short unsigned int);
__simd128_uint16_t __builtin_mve_viwdupq_m_n_uv8hi(__simd128_uint16_t, unsigned int, long long unsigned int, int, short unsigned int);
unsigned int __builtin_mve_viwdupq_m_wb_uv16qi(__simd128_uint8_t, unsigned int, long long unsigned int, int, short unsigned int);
unsigned int __builtin_mve_viwdupq_m_wb_uv4si(__simd128_uint32_t, unsigned int, long long unsigned int, int, short unsigned int);
unsigned int __builtin_mve_viwdupq_m_wb_uv8hi(__simd128_uint16_t, unsigned int, long long unsigned int, int, short unsigned int);
__simd128_uint8_t __builtin_mve_viwdupq_n_uv16qi(unsigned int, long long unsigned int, int);
__simd128_uint32_t __builtin_mve_viwdupq_n_uv4si(unsigned int, long long unsigned int, int);
__simd128_uint16_t __builtin_mve_viwdupq_n_uv8hi(unsigned int, long long unsigned int, int);
unsigned int __builtin_mve_viwdupq_wb_uv16qi(unsigned int, long long unsigned int, int);
unsigned int __builtin_mve_viwdupq_wb_uv4si(unsigned int, long long unsigned int, int);
unsigned int __builtin_mve_viwdupq_wb_uv8hi(unsigned int, long long unsigned int, int);
__simd128_float32_t __builtin_mve_vld1q_fv4sf(int*);
__simd128_float16_t __builtin_mve_vld1q_fv8hf(short int*);
__simd128_int8_t __builtin_mve_vld1q_sv16qi(signed char*);
__simd128_int32_t __builtin_mve_vld1q_sv4si(int*);
__simd128_int16_t __builtin_mve_vld1q_sv8hi(short int*);
__simd128_uint8_t __builtin_mve_vld1q_uv16qi(signed char*);
__simd128_uint32_t __builtin_mve_vld1q_uv4si(int*);
__simd128_uint16_t __builtin_mve_vld1q_uv8hi(short int*);
__builtin_neon_oi __builtin_mve_vld2qv16qi(const signed char*);
__builtin_neon_oi __builtin_mve_vld2qv4sf(const float*);
__builtin_neon_oi __builtin_mve_vld2qv4si(const int*);
__builtin_neon_oi __builtin_mve_vld2qv8hf(const __bf16*);
__builtin_neon_oi __builtin_mve_vld2qv8hi(const short int*);
__builtin_neon_xi __builtin_mve_vld4qv16qi(const signed char*);
__builtin_neon_xi __builtin_mve_vld4qv4sf(const float*);
__builtin_neon_xi __builtin_mve_vld4qv4si(const int*);
__builtin_neon_xi __builtin_mve_vld4qv8hf(const __bf16*);
__builtin_neon_xi __builtin_mve_vld4qv8hi(const short int*);
__simd128_int8_t __builtin_mve_vldrbq_gather_offset_sv16qi(signed char*, __simd128_uint8_t);
__simd128_int32_t __builtin_mve_vldrbq_gather_offset_sv4si(signed char*, __simd128_uint32_t);
__simd128_int16_t __builtin_mve_vldrbq_gather_offset_sv8hi(signed char*, __simd128_uint16_t);
__simd128_uint8_t __builtin_mve_vldrbq_gather_offset_uv16qi(signed char*, __simd128_uint8_t);
__simd128_uint32_t __builtin_mve_vldrbq_gather_offset_uv4si(signed char*, __simd128_uint32_t);
__simd128_uint16_t __builtin_mve_vldrbq_gather_offset_uv8hi(signed char*, __simd128_uint16_t);
__simd128_int8_t __builtin_mve_vldrbq_gather_offset_z_sv16qi(signed char*, __simd128_uint8_t, short unsigned int);
__simd128_int32_t __builtin_mve_vldrbq_gather_offset_z_sv4si(signed char*, __simd128_uint32_t, short unsigned int);
__simd128_int16_t __builtin_mve_vldrbq_gather_offset_z_sv8hi(signed char*, __simd128_uint16_t, short unsigned int);
__simd128_uint8_t __builtin_mve_vldrbq_gather_offset_z_uv16qi(signed char*, __simd128_uint8_t, short unsigned int);
__simd128_uint32_t __builtin_mve_vldrbq_gather_offset_z_uv4si(signed char*, __simd128_uint32_t, short unsigned int);
__simd128_uint16_t __builtin_mve_vldrbq_gather_offset_z_uv8hi(signed char*, __simd128_uint16_t, short unsigned int);
__simd128_int8_t __builtin_mve_vldrbq_sv16qi(signed char*);
__simd128_int32_t __builtin_mve_vldrbq_sv4si(signed char*);
__simd128_int16_t __builtin_mve_vldrbq_sv8hi(signed char*);
__simd128_uint8_t __builtin_mve_vldrbq_uv16qi(signed char*);
__simd128_uint32_t __builtin_mve_vldrbq_uv4si(signed char*);
__simd128_uint16_t __builtin_mve_vldrbq_uv8hi(signed char*);
__simd128_int8_t __builtin_mve_vldrbq_z_sv16qi(signed char*, short unsigned int);
__simd128_int32_t __builtin_mve_vldrbq_z_sv4si(signed char*, short unsigned int);
__simd128_int16_t __builtin_mve_vldrbq_z_sv8hi(signed char*, short unsigned int);
__simd128_uint8_t __builtin_mve_vldrbq_z_uv16qi(signed char*, short unsigned int);
__simd128_uint32_t __builtin_mve_vldrbq_z_uv4si(signed char*, short unsigned int);
__simd128_uint16_t __builtin_mve_vldrbq_z_uv8hi(signed char*, short unsigned int);
__simd128_int64_t __builtin_mve_vldrdq_gather_base_nowb_sv2di(__simd128_uint64_t, int);
__simd128_uint64_t __builtin_mve_vldrdq_gather_base_nowb_uv2di(__simd128_uint64_t, int);
__simd128_int64_t __builtin_mve_vldrdq_gather_base_nowb_z_sv2di(__simd128_uint64_t, int, short unsigned int);
__simd128_uint64_t __builtin_mve_vldrdq_gather_base_nowb_z_uv2di(__simd128_uint64_t, int, short unsigned int);
__simd128_int64_t __builtin_mve_vldrdq_gather_base_sv2di(__simd128_uint64_t, int);
__simd128_uint64_t __builtin_mve_vldrdq_gather_base_uv2di(__simd128_uint64_t, int);
__simd128_uint64_t __builtin_mve_vldrdq_gather_base_wb_sv2di(__simd128_uint64_t, int);
__simd128_uint64_t __builtin_mve_vldrdq_gather_base_wb_uv2di(__simd128_uint64_t, int);
__simd128_uint64_t __builtin_mve_vldrdq_gather_base_wb_z_sv2di(__simd128_uint64_t, int, short unsigned int);
__simd128_uint64_t __builtin_mve_vldrdq_gather_base_wb_z_uv2di(__simd128_uint64_t, int, short unsigned int);
__simd128_int64_t __builtin_mve_vldrdq_gather_base_z_sv2di(__simd128_uint64_t, int, short unsigned int);
__simd128_uint64_t __builtin_mve_vldrdq_gather_base_z_uv2di(__simd128_uint64_t, int, short unsigned int);
__simd128_int64_t __builtin_mve_vldrdq_gather_offset_sv2di(long long int*, __simd128_uint64_t);
__simd128_uint64_t __builtin_mve_vldrdq_gather_offset_uv2di(long long int*, __simd128_uint64_t);
__simd128_int64_t __builtin_mve_vldrdq_gather_offset_z_sv2di(long long int*, __simd128_uint64_t, short unsigned int);
__simd128_uint64_t __builtin_mve_vldrdq_gather_offset_z_uv2di(long long int*, __simd128_uint64_t, short unsigned int);
__simd128_int64_t __builtin_mve_vldrdq_gather_shifted_offset_sv2di(long long int*, __simd128_uint64_t);
__simd128_uint64_t __builtin_mve_vldrdq_gather_shifted_offset_uv2di(long long int*, __simd128_uint64_t);
__simd128_int64_t __builtin_mve_vldrdq_gather_shifted_offset_z_sv2di(long long int*, __simd128_uint64_t, short unsigned int);
__simd128_uint64_t __builtin_mve_vldrdq_gather_shifted_offset_z_uv2di(long long int*, __simd128_uint64_t, short unsigned int);
__simd128_float16_t __builtin_mve_vldrhq_fv8hf(short int*);
__simd128_float16_t __builtin_mve_vldrhq_gather_offset_fv8hf(short int*, __simd128_uint16_t);
__simd128_int32_t __builtin_mve_vldrhq_gather_offset_sv4si(short int*, __simd128_uint32_t);
__simd128_int16_t __builtin_mve_vldrhq_gather_offset_sv8hi(short int*, __simd128_uint16_t);
__simd128_uint32_t __builtin_mve_vldrhq_gather_offset_uv4si(short int*, __simd128_uint32_t);
__simd128_uint16_t __builtin_mve_vldrhq_gather_offset_uv8hi(short int*, __simd128_uint16_t);
__simd128_float16_t __builtin_mve_vldrhq_gather_offset_z_fv8hf(short int*, __simd128_uint16_t, short unsigned int);
__simd128_int32_t __builtin_mve_vldrhq_gather_offset_z_sv4si(short int*, __simd128_uint32_t, short unsigned int);
__simd128_int16_t __builtin_mve_vldrhq_gather_offset_z_sv8hi(short int*, __simd128_uint16_t, short unsigned int);
__simd128_uint32_t __builtin_mve_vldrhq_gather_offset_z_uv4si(short int*, __simd128_uint32_t, short unsigned int);
__simd128_uint16_t __builtin_mve_vldrhq_gather_offset_z_uv8hi(short int*, __simd128_uint16_t, short unsigned int);
__simd128_float16_t __builtin_mve_vldrhq_gather_shifted_offset_fv8hf(short int*, __simd128_uint16_t);
__simd128_int32_t __builtin_mve_vldrhq_gather_shifted_offset_sv4si(short int*, __simd128_uint32_t);
__simd128_int16_t __builtin_mve_vldrhq_gather_shifted_offset_sv8hi(short int*, __simd128_uint16_t);
__simd128_uint32_t __builtin_mve_vldrhq_gather_shifted_offset_uv4si(short int*, __simd128_uint32_t);
__simd128_uint16_t __builtin_mve_vldrhq_gather_shifted_offset_uv8hi(short int*, __simd128_uint16_t);
__simd128_float16_t __builtin_mve_vldrhq_gather_shifted_offset_z_fv8hf(short int*, __simd128_uint16_t, short unsigned int);
__simd128_int32_t __builtin_mve_vldrhq_gather_shifted_offset_z_sv4si(short int*, __simd128_uint32_t, short unsigned int);
__simd128_int16_t __builtin_mve_vldrhq_gather_shifted_offset_z_sv8hi(short int*, __simd128_uint16_t, short unsigned int);
__simd128_uint32_t __builtin_mve_vldrhq_gather_shifted_offset_z_uv4si(short int*, __simd128_uint32_t, short unsigned int);
__simd128_uint16_t __builtin_mve_vldrhq_gather_shifted_offset_z_uv8hi(short int*, __simd128_uint16_t, short unsigned int);
__simd128_int32_t __builtin_mve_vldrhq_sv4si(short int*);
__simd128_int16_t __builtin_mve_vldrhq_sv8hi(short int*);
__simd128_uint32_t __builtin_mve_vldrhq_uv4si(short int*);
__simd128_uint16_t __builtin_mve_vldrhq_uv8hi(short int*);
__simd128_float16_t __builtin_mve_vldrhq_z_fv8hf(short int*, short unsigned int);
__simd128_int32_t __builtin_mve_vldrhq_z_sv4si(short int*, short unsigned int);
__simd128_int16_t __builtin_mve_vldrhq_z_sv8hi(short int*, short unsigned int);
__simd128_uint32_t __builtin_mve_vldrhq_z_uv4si(short int*, short unsigned int);
__simd128_uint16_t __builtin_mve_vldrhq_z_uv8hi(short int*, short unsigned int);
__simd128_float32_t __builtin_mve_vldrwq_fv4sf(int*);
__simd128_float32_t __builtin_mve_vldrwq_gather_base_fv4sf(__simd128_uint32_t, int);
__simd128_float32_t __builtin_mve_vldrwq_gather_base_nowb_fv4sf(__simd128_uint32_t, int);
__simd128_int32_t __builtin_mve_vldrwq_gather_base_nowb_sv4si(__simd128_uint32_t, int);
__simd128_uint32_t __builtin_mve_vldrwq_gather_base_nowb_uv4si(__simd128_uint32_t, int);
__simd128_float32_t __builtin_mve_vldrwq_gather_base_nowb_z_fv4sf(__simd128_uint32_t, int, short unsigned int);
__simd128_int32_t __builtin_mve_vldrwq_gather_base_nowb_z_sv4si(__simd128_uint32_t, int, short unsigned int);
__simd128_uint32_t __builtin_mve_vldrwq_gather_base_nowb_z_uv4si(__simd128_uint32_t, int, short unsigned int);
__simd128_int32_t __builtin_mve_vldrwq_gather_base_sv4si(__simd128_uint32_t, int);
__simd128_uint32_t __builtin_mve_vldrwq_gather_base_uv4si(__simd128_uint32_t, int);
__simd128_uint32_t __builtin_mve_vldrwq_gather_base_wb_fv4sf(__simd128_uint32_t, int);
__simd128_uint32_t __builtin_mve_vldrwq_gather_base_wb_sv4si(__simd128_uint32_t, int);
__simd128_uint32_t __builtin_mve_vldrwq_gather_base_wb_uv4si(__simd128_uint32_t, int);
__simd128_uint32_t __builtin_mve_vldrwq_gather_base_wb_z_fv4sf(__simd128_uint32_t, int, short unsigned int);
__simd128_uint32_t __builtin_mve_vldrwq_gather_base_wb_z_sv4si(__simd128_uint32_t, int, short unsigned int);
__simd128_uint32_t __builtin_mve_vldrwq_gather_base_wb_z_uv4si(__simd128_uint32_t, int, short unsigned int);
__simd128_float32_t __builtin_mve_vldrwq_gather_base_z_fv4sf(__simd128_uint32_t, int, short unsigned int);
__simd128_int32_t __builtin_mve_vldrwq_gather_base_z_sv4si(__simd128_uint32_t, int, short unsigned int);
__simd128_uint32_t __builtin_mve_vldrwq_gather_base_z_uv4si(__simd128_uint32_t, int, short unsigned int);
__simd128_float32_t __builtin_mve_vldrwq_gather_offset_fv4sf(int*, __simd128_uint32_t);
__simd128_int32_t __builtin_mve_vldrwq_gather_offset_sv4si(int*, __simd128_uint32_t);
__simd128_uint32_t __builtin_mve_vldrwq_gather_offset_uv4si(int*, __simd128_uint32_t);
__simd128_float32_t __builtin_mve_vldrwq_gather_offset_z_fv4sf(int*, __simd128_uint32_t, short unsigned int);
__simd128_int32_t __builtin_mve_vldrwq_gather_offset_z_sv4si(int*, __simd128_uint32_t, short unsigned int);
__simd128_uint32_t __builtin_mve_vldrwq_gather_offset_z_uv4si(int*, __simd128_uint32_t, short unsigned int);
__simd128_float32_t __builtin_mve_vldrwq_gather_shifted_offset_fv4sf(int*, __simd128_uint32_t);
__simd128_int32_t __builtin_mve_vldrwq_gather_shifted_offset_sv4si(int*, __simd128_uint32_t);
__simd128_uint32_t __builtin_mve_vldrwq_gather_shifted_offset_uv4si(int*, __simd128_uint32_t);
__simd128_float32_t __builtin_mve_vldrwq_gather_shifted_offset_z_fv4sf(int*, __simd128_uint32_t, short unsigned int);
__simd128_int32_t __builtin_mve_vldrwq_gather_shifted_offset_z_sv4si(int*, __simd128_uint32_t, short unsigned int);
__simd128_uint32_t __builtin_mve_vldrwq_gather_shifted_offset_z_uv4si(int*, __simd128_uint32_t, short unsigned int);
__simd128_int32_t __builtin_mve_vldrwq_sv4si(int*);
__simd128_uint32_t __builtin_mve_vldrwq_uv4si(int*);
__simd128_float32_t __builtin_mve_vldrwq_z_fv4sf(int*, short unsigned int);
__simd128_int32_t __builtin_mve_vldrwq_z_sv4si(int*, short unsigned int);
__simd128_uint32_t __builtin_mve_vldrwq_z_uv4si(int*, short unsigned int);
__simd128_uint8_t __builtin_mve_vmaxaq_m_sv16qi(__simd128_uint8_t, __simd128_int8_t, short unsigned int);
__simd128_uint32_t __builtin_mve_vmaxaq_m_sv4si(__simd128_uint32_t, __simd128_int32_t, short unsigned int);
__simd128_uint16_t __builtin_mve_vmaxaq_m_sv8hi(__simd128_uint16_t, __simd128_int16_t, short unsigned int);
__simd128_uint8_t __builtin_mve_vmaxaq_sv16qi(__simd128_uint8_t, __simd128_int8_t);
__simd128_uint32_t __builtin_mve_vmaxaq_sv4si(__simd128_uint32_t, __simd128_int32_t);
__simd128_uint16_t __builtin_mve_vmaxaq_sv8hi(__simd128_uint16_t, __simd128_int16_t);
unsigned char __builtin_mve_vmaxavq_p_sv16qi(unsigned char, __simd128_int8_t, short unsigned int);
unsigned int __builtin_mve_vmaxavq_p_sv4si(unsigned int, __simd128_int32_t, short unsigned int);
short unsigned int __builtin_mve_vmaxavq_p_sv8hi(short unsigned int, __simd128_int16_t, short unsigned int);
unsigned char __builtin_mve_vmaxavq_sv16qi(unsigned char, __simd128_int8_t);
unsigned int __builtin_mve_vmaxavq_sv4si(unsigned int, __simd128_int32_t);
short unsigned int __builtin_mve_vmaxavq_sv8hi(short unsigned int, __simd128_int16_t);
__simd128_float32_t __builtin_mve_vmaxnmaq_fv4sf(__simd128_float32_t, __simd128_float32_t);
__simd128_float16_t __builtin_mve_vmaxnmaq_fv8hf(__simd128_float16_t, __simd128_float16_t);
__simd128_float32_t __builtin_mve_vmaxnmaq_m_fv4sf(__simd128_float32_t, __simd128_float32_t, short unsigned int);
__simd128_float16_t __builtin_mve_vmaxnmaq_m_fv8hf(__simd128_float16_t, __simd128_float16_t, short unsigned int);
float __builtin_mve_vmaxnmavq_fv4sf(float, __simd128_float32_t);
__bf16 __builtin_mve_vmaxnmavq_fv8hf(__bf16, __simd128_float16_t);
float __builtin_mve_vmaxnmavq_p_fv4sf(float, __simd128_float32_t, short unsigned int);
__bf16 __builtin_mve_vmaxnmavq_p_fv8hf(__bf16, __simd128_float16_t, short unsigned int);
__simd128_float32_t __builtin_mve_vmaxnmq_fv4sf(__simd128_float32_t, __simd128_float32_t);
__simd128_float16_t __builtin_mve_vmaxnmq_fv8hf(__simd128_float16_t, __simd128_float16_t);
__simd128_float32_t __builtin_mve_vmaxnmq_m_fv4sf(__simd128_float32_t, __simd128_float32_t, __simd128_float32_t, short unsigned int);
__simd128_float16_t __builtin_mve_vmaxnmq_m_fv8hf(__simd128_float16_t, __simd128_float16_t, __simd128_float16_t, short unsigned int);
float __builtin_mve_vmaxnmvq_fv4sf(float, __simd128_float32_t);
__bf16 __builtin_mve_vmaxnmvq_fv8hf(__bf16, __simd128_float16_t);
float __builtin_mve_vmaxnmvq_p_fv4sf(float, __simd128_float32_t, short unsigned int);
__bf16 __builtin_mve_vmaxnmvq_p_fv8hf(__bf16, __simd128_float16_t, short unsigned int);
__simd128_int8_t __builtin_mve_vmaxq_m_sv16qi(__simd128_int8_t, __simd128_int8_t, __simd128_int8_t, short unsigned int);
__simd128_int32_t __builtin_mve_vmaxq_m_sv4si(__simd128_int32_t, __simd128_int32_t, __simd128_int32_t, short unsigned int);
__simd128_int16_t __builtin_mve_vmaxq_m_sv8hi(__simd128_int16_t, __simd128_int16_t, __simd128_int16_t, short unsigned int);
__simd128_uint8_t __builtin_mve_vmaxq_m_uv16qi(__simd128_uint8_t, __simd128_uint8_t, __simd128_uint8_t, short unsigned int);
__simd128_uint32_t __builtin_mve_vmaxq_m_uv4si(__simd128_uint32_t, __simd128_uint32_t, __simd128_uint32_t, short unsigned int);
__simd128_uint16_t __builtin_mve_vmaxq_m_uv8hi(__simd128_uint16_t, __simd128_uint16_t, __simd128_uint16_t, short unsigned int);
__simd128_int8_t __builtin_mve_vmaxq_sv16qi(__simd128_int8_t, __simd128_int8_t);
__simd128_int32_t __builtin_mve_vmaxq_sv4si(__simd128_int32_t, __simd128_int32_t);
__simd128_int16_t __builtin_mve_vmaxq_sv8hi(__simd128_int16_t, __simd128_int16_t);
__simd128_uint8_t __builtin_mve_vmaxq_uv16qi(__simd128_uint8_t, __simd128_uint8_t);
__simd128_uint32_t __builtin_mve_vmaxq_uv4si(__simd128_uint32_t, __simd128_uint32_t);
__simd128_uint16_t __builtin_mve_vmaxq_uv8hi(__simd128_uint16_t, __simd128_uint16_t);
signed char __builtin_mve_vmaxvq_p_sv16qi(signed char, __simd128_int8_t, short unsigned int);
int __builtin_mve_vmaxvq_p_sv4si(int, __simd128_int32_t, short unsigned int);
short int __builtin_mve_vmaxvq_p_sv8hi(short int, __simd128_int16_t, short unsigned int);
unsigned char __builtin_mve_vmaxvq_p_uv16qi(unsigned char, __simd128_uint8_t, short unsigned int);
unsigned int __builtin_mve_vmaxvq_p_uv4si(unsigned int, __simd128_uint32_t, short unsigned int);
short unsigned int __builtin_mve_vmaxvq_p_uv8hi(short unsigned int, __simd128_uint16_t, short unsigned int);
signed char __builtin_mve_vmaxvq_sv16qi(signed char, __simd128_int8_t);
int __builtin_mve_vmaxvq_sv4si(int, __simd128_int32_t);
short int __builtin_mve_vmaxvq_sv8hi(short int, __simd128_int16_t);
unsigned char __builtin_mve_vmaxvq_uv16qi(unsigned char, __simd128_uint8_t);
unsigned int __builtin_mve_vmaxvq_uv4si(unsigned int, __simd128_uint32_t);
short unsigned int __builtin_mve_vmaxvq_uv8hi(short unsigned int, __simd128_uint16_t);
__simd128_uint8_t __builtin_mve_vminaq_m_sv16qi(__simd128_uint8_t, __simd128_int8_t, short unsigned int);
__simd128_uint32_t __builtin_mve_vminaq_m_sv4si(__simd128_uint32_t, __simd128_int32_t, short unsigned int);
__simd128_uint16_t __builtin_mve_vminaq_m_sv8hi(__simd128_uint16_t, __simd128_int16_t, short unsigned int);
__simd128_uint8_t __builtin_mve_vminaq_sv16qi(__simd128_uint8_t, __simd128_int8_t);
__simd128_uint32_t __builtin_mve_vminaq_sv4si(__simd128_uint32_t, __simd128_int32_t);
__simd128_uint16_t __builtin_mve_vminaq_sv8hi(__simd128_uint16_t, __simd128_int16_t);
unsigned char __builtin_mve_vminavq_p_sv16qi(unsigned char, __simd128_int8_t, short unsigned int);
unsigned int __builtin_mve_vminavq_p_sv4si(unsigned int, __simd128_int32_t, short unsigned int);
short unsigned int __builtin_mve_vminavq_p_sv8hi(short unsigned int, __simd128_int16_t, short unsigned int);
unsigned char __builtin_mve_vminavq_sv16qi(unsigned char, __simd128_int8_t);
unsigned int __builtin_mve_vminavq_sv4si(unsigned int, __simd128_int32_t);
short unsigned int __builtin_mve_vminavq_sv8hi(short unsigned int, __simd128_int16_t);
__simd128_float32_t __builtin_mve_vminnmaq_fv4sf(__simd128_float32_t, __simd128_float32_t);
__simd128_float16_t __builtin_mve_vminnmaq_fv8hf(__simd128_float16_t, __simd128_float16_t);
__simd128_float32_t __builtin_mve_vminnmaq_m_fv4sf(__simd128_float32_t, __simd128_float32_t, short unsigned int);
__simd128_float16_t __builtin_mve_vminnmaq_m_fv8hf(__simd128_float16_t, __simd128_float16_t, short unsigned int);
float __builtin_mve_vminnmavq_fv4sf(float, __simd128_float32_t);
__bf16 __builtin_mve_vminnmavq_fv8hf(__bf16, __simd128_float16_t);
float __builtin_mve_vminnmavq_p_fv4sf(float, __simd128_float32_t, short unsigned int);
__bf16 __builtin_mve_vminnmavq_p_fv8hf(__bf16, __simd128_float16_t, short unsigned int);
__simd128_float32_t __builtin_mve_vminnmq_fv4sf(__simd128_float32_t, __simd128_float32_t);
__simd128_float16_t __builtin_mve_vminnmq_fv8hf(__simd128_float16_t, __simd128_float16_t);
__simd128_float32_t __builtin_mve_vminnmq_m_fv4sf(__simd128_float32_t, __simd128_float32_t, __simd128_float32_t, short unsigned int);
__simd128_float16_t __builtin_mve_vminnmq_m_fv8hf(__simd128_float16_t, __simd128_float16_t, __simd128_float16_t, short unsigned int);
float __builtin_mve_vminnmvq_fv4sf(float, __simd128_float32_t);
__bf16 __builtin_mve_vminnmvq_fv8hf(__bf16, __simd128_float16_t);
float __builtin_mve_vminnmvq_p_fv4sf(float, __simd128_float32_t, short unsigned int);
__bf16 __builtin_mve_vminnmvq_p_fv8hf(__bf16, __simd128_float16_t, short unsigned int);
__simd128_int8_t __builtin_mve_vminq_m_sv16qi(__simd128_int8_t, __simd128_int8_t, __simd128_int8_t, short unsigned int);
__simd128_int32_t __builtin_mve_vminq_m_sv4si(__simd128_int32_t, __simd128_int32_t, __simd128_int32_t, short unsigned int);
__simd128_int16_t __builtin_mve_vminq_m_sv8hi(__simd128_int16_t, __simd128_int16_t, __simd128_int16_t, short unsigned int);
__simd128_uint8_t __builtin_mve_vminq_m_uv16qi(__simd128_uint8_t, __simd128_uint8_t, __simd128_uint8_t, short unsigned int);
__simd128_uint32_t __builtin_mve_vminq_m_uv4si(__simd128_uint32_t, __simd128_uint32_t, __simd128_uint32_t, short unsigned int);
__simd128_uint16_t __builtin_mve_vminq_m_uv8hi(__simd128_uint16_t, __simd128_uint16_t, __simd128_uint16_t, short unsigned int);
__simd128_int8_t __builtin_mve_vminq_sv16qi(__simd128_int8_t, __simd128_int8_t);
__simd128_int32_t __builtin_mve_vminq_sv4si(__simd128_int32_t, __simd128_int32_t);
__simd128_int16_t __builtin_mve_vminq_sv8hi(__simd128_int16_t, __simd128_int16_t);
__simd128_uint8_t __builtin_mve_vminq_uv16qi(__simd128_uint8_t, __simd128_uint8_t);
__simd128_uint32_t __builtin_mve_vminq_uv4si(__simd128_uint32_t, __simd128_uint32_t);
__simd128_uint16_t __builtin_mve_vminq_uv8hi(__simd128_uint16_t, __simd128_uint16_t);
signed char __builtin_mve_vminvq_p_sv16qi(signed char, __simd128_int8_t, short unsigned int);
int __builtin_mve_vminvq_p_sv4si(int, __simd128_int32_t, short unsigned int);
short int __builtin_mve_vminvq_p_sv8hi(short int, __simd128_int16_t, short unsigned int);
unsigned char __builtin_mve_vminvq_p_uv16qi(unsigned char, __simd128_uint8_t, short unsigned int);
unsigned int __builtin_mve_vminvq_p_uv4si(unsigned int, __simd128_uint32_t, short unsigned int);
short unsigned int __builtin_mve_vminvq_p_uv8hi(short unsigned int, __simd128_uint16_t, short unsigned int);
signed char __builtin_mve_vminvq_sv16qi(signed char, __simd128_int8_t);
int __builtin_mve_vminvq_sv4si(int, __simd128_int32_t);
short int __builtin_mve_vminvq_sv8hi(short int, __simd128_int16_t);
unsigned char __builtin_mve_vminvq_uv16qi(unsigned char, __simd128_uint8_t);
unsigned int __builtin_mve_vminvq_uv4si(unsigned int, __simd128_uint32_t);
short unsigned int __builtin_mve_vminvq_uv8hi(short unsigned int, __simd128_uint16_t);
int __builtin_mve_vmladavaq_p_sv16qi(int, __simd128_int8_t, __simd128_int8_t, short unsigned int);
int __builtin_mve_vmladavaq_p_sv4si(int, __simd128_int32_t, __simd128_int32_t, short unsigned int);
int __builtin_mve_vmladavaq_p_sv8hi(int, __simd128_int16_t, __simd128_int16_t, short unsigned int);
unsigned int __builtin_mve_vmladavaq_p_uv16qi(unsigned int, __simd128_uint8_t, __simd128_uint8_t, short unsigned int);
unsigned int __builtin_mve_vmladavaq_p_uv4si(unsigned int, __simd128_uint32_t, __simd128_uint32_t, short unsigned int);
unsigned int __builtin_mve_vmladavaq_p_uv8hi(unsigned int, __simd128_uint16_t, __simd128_uint16_t, short unsigned int);
int __builtin_mve_vmladavaq_sv16qi(int, __simd128_int8_t, __simd128_int8_t);
int __builtin_mve_vmladavaq_sv4si(int, __simd128_int32_t, __simd128_int32_t);
int __builtin_mve_vmladavaq_sv8hi(int, __simd128_int16_t, __simd128_int16_t);
unsigned int __builtin_mve_vmladavaq_uv16qi(unsigned int, __simd128_uint8_t, __simd128_uint8_t);
unsigned int __builtin_mve_vmladavaq_uv4si(unsigned int, __simd128_uint32_t, __simd128_uint32_t);
unsigned int __builtin_mve_vmladavaq_uv8hi(unsigned int, __simd128_uint16_t, __simd128_uint16_t);
int __builtin_mve_vmladavaxq_p_sv16qi(int, __simd128_int8_t, __simd128_int8_t, short unsigned int);
int __builtin_mve_vmladavaxq_p_sv4si(int, __simd128_int32_t, __simd128_int32_t, short unsigned int);
int __builtin_mve_vmladavaxq_p_sv8hi(int, __simd128_int16_t, __simd128_int16_t, short unsigned int);
int __builtin_mve_vmladavaxq_sv16qi(int, __simd128_int8_t, __simd128_int8_t);
int __builtin_mve_vmladavaxq_sv4si(int, __simd128_int32_t, __simd128_int32_t);
int __builtin_mve_vmladavaxq_sv8hi(int, __simd128_int16_t, __simd128_int16_t);
int __builtin_mve_vmladavq_p_sv16qi(__simd128_int8_t, __simd128_int8_t, short unsigned int);
int __builtin_mve_vmladavq_p_sv4si(__simd128_int32_t, __simd128_int32_t, short unsigned int);
int __builtin_mve_vmladavq_p_sv8hi(__simd128_int16_t, __simd128_int16_t, short unsigned int);
unsigned int __builtin_mve_vmladavq_p_uv16qi(__simd128_uint8_t, __simd128_uint8_t, short unsigned int);
unsigned int __builtin_mve_vmladavq_p_uv4si(__simd128_uint32_t, __simd128_uint32_t, short unsigned int);
unsigned int __builtin_mve_vmladavq_p_uv8hi(__simd128_uint16_t, __simd128_uint16_t, short unsigned int);
int __builtin_mve_vmladavq_sv16qi(__simd128_int8_t, __simd128_int8_t);
int __builtin_mve_vmladavq_sv4si(__simd128_int32_t, __simd128_int32_t);
int __builtin_mve_vmladavq_sv8hi(__simd128_int16_t, __simd128_int16_t);
unsigned int __builtin_mve_vmladavq_uv16qi(__simd128_uint8_t, __simd128_uint8_t);
unsigned int __builtin_mve_vmladavq_uv4si(__simd128_uint32_t, __simd128_uint32_t);
unsigned int __builtin_mve_vmladavq_uv8hi(__simd128_uint16_t, __simd128_uint16_t);
int __builtin_mve_vmladavxq_p_sv16qi(__simd128_int8_t, __simd128_int8_t, short unsigned int);
int __builtin_mve_vmladavxq_p_sv4si(__simd128_int32_t, __simd128_int32_t, short unsigned int);
int __builtin_mve_vmladavxq_p_sv8hi(__simd128_int16_t, __simd128_int16_t, short unsigned int);
int __builtin_mve_vmladavxq_sv16qi(__simd128_int8_t, __simd128_int8_t);
int __builtin_mve_vmladavxq_sv4si(__simd128_int32_t, __simd128_int32_t);
int __builtin_mve_vmladavxq_sv8hi(__simd128_int16_t, __simd128_int16_t);
long long int __builtin_mve_vmlaldavaq_p_sv4si(long long int, __simd128_int32_t, __simd128_int32_t, short unsigned int);
long long int __builtin_mve_vmlaldavaq_p_sv8hi(long long int, __simd128_int16_t, __simd128_int16_t, short unsigned int);
long long unsigned int __builtin_mve_vmlaldavaq_p_uv4si(long long unsigned int, __simd128_uint32_t, __simd128_uint32_t, short unsigned int);
long long unsigned int __builtin_mve_vmlaldavaq_p_uv8hi(long long unsigned int, __simd128_uint16_t, __simd128_uint16_t, short unsigned int);
long long int __builtin_mve_vmlaldavaq_sv4si(long long int, __simd128_int32_t, __simd128_int32_t);
long long int __builtin_mve_vmlaldavaq_sv8hi(long long int, __simd128_int16_t, __simd128_int16_t);
long long unsigned int __builtin_mve_vmlaldavaq_uv4si(long long unsigned int, __simd128_uint32_t, __simd128_uint32_t);
long long unsigned int __builtin_mve_vmlaldavaq_uv8hi(long long unsigned int, __simd128_uint16_t, __simd128_uint16_t);
long long int __builtin_mve_vmlaldavaxq_p_sv4si(long long int, __simd128_int32_t, __simd128_int32_t, short unsigned int);
long long int __builtin_mve_vmlaldavaxq_p_sv8hi(long long int, __simd128_int16_t, __simd128_int16_t, short unsigned int);
long long unsigned int __builtin_mve_vmlaldavaxq_p_uv4si(long long unsigned int, __simd128_uint32_t, __simd128_uint32_t, short unsigned int);
long long unsigned int __builtin_mve_vmlaldavaxq_p_uv8hi(long long unsigned int, __simd128_uint16_t, __simd128_uint16_t, short unsigned int);
long long int __builtin_mve_vmlaldavaxq_sv4si(long long int, __simd128_int32_t, __simd128_int32_t);
long long int __builtin_mve_vmlaldavaxq_sv8hi(long long int, __simd128_int16_t, __simd128_int16_t);
long long int __builtin_mve_vmlaldavq_p_sv4si(__simd128_int32_t, __simd128_int32_t, short unsigned int);
long long int __builtin_mve_vmlaldavq_p_sv8hi(__simd128_int16_t, __simd128_int16_t, short unsigned int);
long long unsigned int __builtin_mve_vmlaldavq_p_uv4si(__simd128_uint32_t, __simd128_uint32_t, short unsigned int);
long long unsigned int __builtin_mve_vmlaldavq_p_uv8hi(__simd128_uint16_t, __simd128_uint16_t, short unsigned int);
long long int __builtin_mve_vmlaldavq_sv4si(__simd128_int32_t, __simd128_int32_t);
long long int __builtin_mve_vmlaldavq_sv8hi(__simd128_int16_t, __simd128_int16_t);
long long unsigned int __builtin_mve_vmlaldavq_uv4si(__simd128_uint32_t, __simd128_uint32_t);
long long unsigned int __builtin_mve_vmlaldavq_uv8hi(__simd128_uint16_t, __simd128_uint16_t);
long long int __builtin_mve_vmlaldavxq_p_sv4si(__simd128_int32_t, __simd128_int32_t, short unsigned int);
long long int __builtin_mve_vmlaldavxq_p_sv8hi(__simd128_int16_t, __simd128_int16_t, short unsigned int);
long long int __builtin_mve_vmlaldavxq_sv4si(__simd128_int32_t, __simd128_int32_t);
long long int __builtin_mve_vmlaldavxq_sv8hi(__simd128_int16_t, __simd128_int16_t);
__simd128_int8_t __builtin_mve_vmlaq_m_n_sv16qi(__simd128_int8_t, __simd128_int8_t, signed char, short unsigned int);
__simd128_int32_t __builtin_mve_vmlaq_m_n_sv4si(__simd128_int32_t, __simd128_int32_t, int, short unsigned int);
__simd128_int16_t __builtin_mve_vmlaq_m_n_sv8hi(__simd128_int16_t, __simd128_int16_t, short int, short unsigned int);
__simd128_uint8_t __builtin_mve_vmlaq_m_n_uv16qi(__simd128_uint8_t, __simd128_uint8_t, unsigned char, short unsigned int);
__simd128_uint32_t __builtin_mve_vmlaq_m_n_uv4si(__simd128_uint32_t, __simd128_uint32_t, unsigned int, short unsigned int);
__simd128_uint16_t __builtin_mve_vmlaq_m_n_uv8hi(__simd128_uint16_t, __simd128_uint16_t, short unsigned int, short unsigned int);
__simd128_int8_t __builtin_mve_vmlaq_n_sv16qi(__simd128_int8_t, __simd128_int8_t, signed char);
__simd128_int32_t __builtin_mve_vmlaq_n_sv4si(__simd128_int32_t, __simd128_int32_t, int);
__simd128_int16_t __builtin_mve_vmlaq_n_sv8hi(__simd128_int16_t, __simd128_int16_t, short int);
__simd128_uint8_t __builtin_mve_vmlaq_n_uv16qi(__simd128_uint8_t, __simd128_uint8_t, unsigned char);
__simd128_uint32_t __builtin_mve_vmlaq_n_uv4si(__simd128_uint32_t, __simd128_uint32_t, unsigned int);
__simd128_uint16_t __builtin_mve_vmlaq_n_uv8hi(__simd128_uint16_t, __simd128_uint16_t, short unsigned int);
__simd128_int8_t __builtin_mve_vmlasq_m_n_sv16qi(__simd128_int8_t, __simd128_int8_t, signed char, short unsigned int);
__simd128_int32_t __builtin_mve_vmlasq_m_n_sv4si(__simd128_int32_t, __simd128_int32_t, int, short unsigned int);
__simd128_int16_t __builtin_mve_vmlasq_m_n_sv8hi(__simd128_int16_t, __simd128_int16_t, short int, short unsigned int);
__simd128_uint8_t __builtin_mve_vmlasq_m_n_uv16qi(__simd128_uint8_t, __simd128_uint8_t, unsigned char, short unsigned int);
__simd128_uint32_t __builtin_mve_vmlasq_m_n_uv4si(__simd128_uint32_t, __simd128_uint32_t, unsigned int, short unsigned int);
__simd128_uint16_t __builtin_mve_vmlasq_m_n_uv8hi(__simd128_uint16_t, __simd128_uint16_t, short unsigned int, short unsigned int);
__simd128_int8_t __builtin_mve_vmlasq_n_sv16qi(__simd128_int8_t, __simd128_int8_t, signed char);
__simd128_int32_t __builtin_mve_vmlasq_n_sv4si(__simd128_int32_t, __simd128_int32_t, int);
__simd128_int16_t __builtin_mve_vmlasq_n_sv8hi(__simd128_int16_t, __simd128_int16_t, short int);
__simd128_uint8_t __builtin_mve_vmlasq_n_uv16qi(__simd128_uint8_t, __simd128_uint8_t, unsigned char);
__simd128_uint32_t __builtin_mve_vmlasq_n_uv4si(__simd128_uint32_t, __simd128_uint32_t, unsigned int);
__simd128_uint16_t __builtin_mve_vmlasq_n_uv8hi(__simd128_uint16_t, __simd128_uint16_t, short unsigned int);
int __builtin_mve_vmlsdavaq_p_sv16qi(int, __simd128_int8_t, __simd128_int8_t, short unsigned int);
int __builtin_mve_vmlsdavaq_p_sv4si(int, __simd128_int32_t, __simd128_int32_t, short unsigned int);
int __builtin_mve_vmlsdavaq_p_sv8hi(int, __simd128_int16_t, __simd128_int16_t, short unsigned int);
int __builtin_mve_vmlsdavaq_sv16qi(int, __simd128_int8_t, __simd128_int8_t);
int __builtin_mve_vmlsdavaq_sv4si(int, __simd128_int32_t, __simd128_int32_t);
int __builtin_mve_vmlsdavaq_sv8hi(int, __simd128_int16_t, __simd128_int16_t);
int __builtin_mve_vmlsdavaxq_p_sv16qi(int, __simd128_int8_t, __simd128_int8_t, short unsigned int);
int __builtin_mve_vmlsdavaxq_p_sv4si(int, __simd128_int32_t, __simd128_int32_t, short unsigned int);
int __builtin_mve_vmlsdavaxq_p_sv8hi(int, __simd128_int16_t, __simd128_int16_t, short unsigned int);
int __builtin_mve_vmlsdavaxq_sv16qi(int, __simd128_int8_t, __simd128_int8_t);
int __builtin_mve_vmlsdavaxq_sv4si(int, __simd128_int32_t, __simd128_int32_t);
int __builtin_mve_vmlsdavaxq_sv8hi(int, __simd128_int16_t, __simd128_int16_t);
int __builtin_mve_vmlsdavq_p_sv16qi(__simd128_int8_t, __simd128_int8_t, short unsigned int);
int __builtin_mve_vmlsdavq_p_sv4si(__simd128_int32_t, __simd128_int32_t, short unsigned int);
int __builtin_mve_vmlsdavq_p_sv8hi(__simd128_int16_t, __simd128_int16_t, short unsigned int);
int __builtin_mve_vmlsdavq_sv16qi(__simd128_int8_t, __simd128_int8_t);
int __builtin_mve_vmlsdavq_sv4si(__simd128_int32_t, __simd128_int32_t);
int __builtin_mve_vmlsdavq_sv8hi(__simd128_int16_t, __simd128_int16_t);
int __builtin_mve_vmlsdavxq_p_sv16qi(__simd128_int8_t, __simd128_int8_t, short unsigned int);
int __builtin_mve_vmlsdavxq_p_sv4si(__simd128_int32_t, __simd128_int32_t, short unsigned int);
int __builtin_mve_vmlsdavxq_p_sv8hi(__simd128_int16_t, __simd128_int16_t, short unsigned int);
int __builtin_mve_vmlsdavxq_sv16qi(__simd128_int8_t, __simd128_int8_t);
int __builtin_mve_vmlsdavxq_sv4si(__simd128_int32_t, __simd128_int32_t);
int __builtin_mve_vmlsdavxq_sv8hi(__simd128_int16_t, __simd128_int16_t);
long long int __builtin_mve_vmlsldavaq_p_sv4si(long long int, __simd128_int32_t, __simd128_int32_t, short unsigned int);
long long int __builtin_mve_vmlsldavaq_p_sv8hi(long long int, __simd128_int16_t, __simd128_int16_t, short unsigned int);
long long int __builtin_mve_vmlsldavaq_sv4si(long long int, __simd128_int32_t, __simd128_int32_t);
long long int __builtin_mve_vmlsldavaq_sv8hi(long long int, __simd128_int16_t, __simd128_int16_t);
long long int __builtin_mve_vmlsldavaxq_p_sv4si(long long int, __simd128_int32_t, __simd128_int32_t, short unsigned int);
long long int __builtin_mve_vmlsldavaxq_p_sv8hi(long long int, __simd128_int16_t, __simd128_int16_t, short unsigned int);
long long int __builtin_mve_vmlsldavaxq_sv4si(long long int, __simd128_int32_t, __simd128_int32_t);
long long int __builtin_mve_vmlsldavaxq_sv8hi(long long int, __simd128_int16_t, __simd128_int16_t);
long long int __builtin_mve_vmlsldavq_p_sv4si(__simd128_int32_t, __simd128_int32_t, short unsigned int);
long long int __builtin_mve_vmlsldavq_p_sv8hi(__simd128_int16_t, __simd128_int16_t, short unsigned int);
long long int __builtin_mve_vmlsldavq_sv4si(__simd128_int32_t, __simd128_int32_t);
long long int __builtin_mve_vmlsldavq_sv8hi(__simd128_int16_t, __simd128_int16_t);
long long int __builtin_mve_vmlsldavxq_p_sv4si(__simd128_int32_t, __simd128_int32_t, short unsigned int);
long long int __builtin_mve_vmlsldavxq_p_sv8hi(__simd128_int16_t, __simd128_int16_t, short unsigned int);
long long int __builtin_mve_vmlsldavxq_sv4si(__simd128_int32_t, __simd128_int32_t);
long long int __builtin_mve_vmlsldavxq_sv8hi(__simd128_int16_t, __simd128_int16_t);
__simd128_int16_t __builtin_mve_vmovlbq_m_sv16qi(__simd128_int16_t, __simd128_int8_t, short unsigned int);
__simd128_int32_t __builtin_mve_vmovlbq_m_sv8hi(__simd128_int32_t, __simd128_int16_t, short unsigned int);
__simd128_uint16_t __builtin_mve_vmovlbq_m_uv16qi(__simd128_uint16_t, __simd128_uint8_t, short unsigned int);
__simd128_uint32_t __builtin_mve_vmovlbq_m_uv8hi(__simd128_uint32_t, __simd128_uint16_t, short unsigned int);
__simd128_int16_t __builtin_mve_vmovlbq_sv16qi(__simd128_int8_t);
__simd128_int32_t __builtin_mve_vmovlbq_sv8hi(__simd128_int16_t);
__simd128_uint16_t __builtin_mve_vmovlbq_uv16qi(__simd128_uint8_t);
__simd128_uint32_t __builtin_mve_vmovlbq_uv8hi(__simd128_uint16_t);
__simd128_int16_t __builtin_mve_vmovltq_m_sv16qi(__simd128_int16_t, __simd128_int8_t, short unsigned int);
__simd128_int32_t __builtin_mve_vmovltq_m_sv8hi(__simd128_int32_t, __simd128_int16_t, short unsigned int);
__simd128_uint16_t __builtin_mve_vmovltq_m_uv16qi(__simd128_uint16_t, __simd128_uint8_t, short unsigned int);
__simd128_uint32_t __builtin_mve_vmovltq_m_uv8hi(__simd128_uint32_t, __simd128_uint16_t, short unsigned int);
__simd128_int16_t __builtin_mve_vmovltq_sv16qi(__simd128_int8_t);
__simd128_int32_t __builtin_mve_vmovltq_sv8hi(__simd128_int16_t);
__simd128_uint16_t __builtin_mve_vmovltq_uv16qi(__simd128_uint8_t);
__simd128_uint32_t __builtin_mve_vmovltq_uv8hi(__simd128_uint16_t);
__simd128_int16_t __builtin_mve_vmovnbq_m_sv4si(__simd128_int16_t, __simd128_int32_t, short unsigned int);
__simd128_int8_t __builtin_mve_vmovnbq_m_sv8hi(__simd128_int8_t, __simd128_int16_t, short unsigned int);
__simd128_uint16_t __builtin_mve_vmovnbq_m_uv4si(__simd128_uint16_t, __simd128_uint32_t, short unsigned int);
__simd128_uint8_t __builtin_mve_vmovnbq_m_uv8hi(__simd128_uint8_t, __simd128_uint16_t, short unsigned int);
__simd128_int16_t __builtin_mve_vmovnbq_sv4si(__simd128_int16_t, __simd128_int32_t);
__simd128_int8_t __builtin_mve_vmovnbq_sv8hi(__simd128_int8_t, __simd128_int16_t);
__simd128_uint16_t __builtin_mve_vmovnbq_uv4si(__simd128_uint16_t, __simd128_uint32_t);
__simd128_uint8_t __builtin_mve_vmovnbq_uv8hi(__simd128_uint8_t, __simd128_uint16_t);
__simd128_int16_t __builtin_mve_vmovntq_m_sv4si(__simd128_int16_t, __simd128_int32_t, short unsigned int);
__simd128_int8_t __builtin_mve_vmovntq_m_sv8hi(__simd128_int8_t, __simd128_int16_t, short unsigned int);
__simd128_uint16_t __builtin_mve_vmovntq_m_uv4si(__simd128_uint16_t, __simd128_uint32_t, short unsigned int);
__simd128_uint8_t __builtin_mve_vmovntq_m_uv8hi(__simd128_uint8_t, __simd128_uint16_t, short unsigned int);
__simd128_int16_t __builtin_mve_vmovntq_sv4si(__simd128_int16_t, __simd128_int32_t);
__simd128_int8_t __builtin_mve_vmovntq_sv8hi(__simd128_int8_t, __simd128_int16_t);
__simd128_uint16_t __builtin_mve_vmovntq_uv4si(__simd128_uint16_t, __simd128_uint32_t);
__simd128_uint8_t __builtin_mve_vmovntq_uv8hi(__simd128_uint8_t, __simd128_uint16_t);
__simd128_int8_t __builtin_mve_vmulhq_m_sv16qi(__simd128_int8_t, __simd128_int8_t, __simd128_int8_t, short unsigned int);
__simd128_int32_t __builtin_mve_vmulhq_m_sv4si(__simd128_int32_t, __simd128_int32_t, __simd128_int32_t, short unsigned int);
__simd128_int16_t __builtin_mve_vmulhq_m_sv8hi(__simd128_int16_t, __simd128_int16_t, __simd128_int16_t, short unsigned int);
__simd128_uint8_t __builtin_mve_vmulhq_m_uv16qi(__simd128_uint8_t, __simd128_uint8_t, __simd128_uint8_t, short unsigned int);
__simd128_uint32_t __builtin_mve_vmulhq_m_uv4si(__simd128_uint32_t, __simd128_uint32_t, __simd128_uint32_t, short unsigned int);
__simd128_uint16_t __builtin_mve_vmulhq_m_uv8hi(__simd128_uint16_t, __simd128_uint16_t, __simd128_uint16_t, short unsigned int);
__simd128_int8_t __builtin_mve_vmulhq_sv16qi(__simd128_int8_t, __simd128_int8_t);
__simd128_int32_t __builtin_mve_vmulhq_sv4si(__simd128_int32_t, __simd128_int32_t);
__simd128_int16_t __builtin_mve_vmulhq_sv8hi(__simd128_int16_t, __simd128_int16_t);
__simd128_uint8_t __builtin_mve_vmulhq_uv16qi(__simd128_uint8_t, __simd128_uint8_t);
__simd128_uint32_t __builtin_mve_vmulhq_uv4si(__simd128_uint32_t, __simd128_uint32_t);
__simd128_uint16_t __builtin_mve_vmulhq_uv8hi(__simd128_uint16_t, __simd128_uint16_t);
__simd128_int16_t __builtin_mve_vmullbq_int_m_sv16qi(__simd128_int16_t, __simd128_int8_t, __simd128_int8_t, short unsigned int);
__simd128_int64_t __builtin_mve_vmullbq_int_m_sv4si(__simd128_int64_t, __simd128_int32_t, __simd128_int32_t, short unsigned int);
__simd128_int32_t __builtin_mve_vmullbq_int_m_sv8hi(__simd128_int32_t, __simd128_int16_t, __simd128_int16_t, short unsigned int);
__simd128_uint16_t __builtin_mve_vmullbq_int_m_uv16qi(__simd128_uint16_t, __simd128_uint8_t, __simd128_uint8_t, short unsigned int);
__simd128_uint64_t __builtin_mve_vmullbq_int_m_uv4si(__simd128_uint64_t, __simd128_uint32_t, __simd128_uint32_t, short unsigned int);
__simd128_uint32_t __builtin_mve_vmullbq_int_m_uv8hi(__simd128_uint32_t, __simd128_uint16_t, __simd128_uint16_t, short unsigned int);
__simd128_int16_t __builtin_mve_vmullbq_int_sv16qi(__simd128_int8_t, __simd128_int8_t);
__simd128_int64_t __builtin_mve_vmullbq_int_sv4si(__simd128_int32_t, __simd128_int32_t);
__simd128_int32_t __builtin_mve_vmullbq_int_sv8hi(__simd128_int16_t, __simd128_int16_t);
__simd128_uint16_t __builtin_mve_vmullbq_int_uv16qi(__simd128_uint8_t, __simd128_uint8_t);
__simd128_uint64_t __builtin_mve_vmullbq_int_uv4si(__simd128_uint32_t, __simd128_uint32_t);
__simd128_uint32_t __builtin_mve_vmullbq_int_uv8hi(__simd128_uint16_t, __simd128_uint16_t);
__simd128_uint16_t __builtin_mve_vmullbq_poly_m_pv16qi(__simd128_uint16_t, __simd128_uint8_t, __simd128_uint8_t, short unsigned int);
__simd128_uint32_t __builtin_mve_vmullbq_poly_m_pv8hi(__simd128_uint32_t, __simd128_uint16_t, __simd128_uint16_t, short unsigned int);
__simd128_uint16_t __builtin_mve_vmullbq_poly_pv16qi(__simd128_uint8_t, __simd128_uint8_t);
__simd128_uint32_t __builtin_mve_vmullbq_poly_pv8hi(__simd128_uint16_t, __simd128_uint16_t);
__simd128_int16_t __builtin_mve_vmulltq_int_m_sv16qi(__simd128_int16_t, __simd128_int8_t, __simd128_int8_t, short unsigned int);
__simd128_int64_t __builtin_mve_vmulltq_int_m_sv4si(__simd128_int64_t, __simd128_int32_t, __simd128_int32_t, short unsigned int);
__simd128_int32_t __builtin_mve_vmulltq_int_m_sv8hi(__simd128_int32_t, __simd128_int16_t, __simd128_int16_t, short unsigned int);
__simd128_uint16_t __builtin_mve_vmulltq_int_m_uv16qi(__simd128_uint16_t, __simd128_uint8_t, __simd128_uint8_t, short unsigned int);
__simd128_uint64_t __builtin_mve_vmulltq_int_m_uv4si(__simd128_uint64_t, __simd128_uint32_t, __simd128_uint32_t, short unsigned int);
__simd128_uint32_t __builtin_mve_vmulltq_int_m_uv8hi(__simd128_uint32_t, __simd128_uint16_t, __simd128_uint16_t, short unsigned int);
__simd128_int16_t __builtin_mve_vmulltq_int_sv16qi(__simd128_int8_t, __simd128_int8_t);
__simd128_int64_t __builtin_mve_vmulltq_int_sv4si(__simd128_int32_t, __simd128_int32_t);
__simd128_int32_t __builtin_mve_vmulltq_int_sv8hi(__simd128_int16_t, __simd128_int16_t);
__simd128_uint16_t __builtin_mve_vmulltq_int_uv16qi(__simd128_uint8_t, __simd128_uint8_t);
__simd128_uint64_t __builtin_mve_vmulltq_int_uv4si(__simd128_uint32_t, __simd128_uint32_t);
__simd128_uint32_t __builtin_mve_vmulltq_int_uv8hi(__simd128_uint16_t, __simd128_uint16_t);
__simd128_uint16_t __builtin_mve_vmulltq_poly_m_pv16qi(__simd128_uint16_t, __simd128_uint8_t, __simd128_uint8_t, short unsigned int);
__simd128_uint32_t __builtin_mve_vmulltq_poly_m_pv8hi(__simd128_uint32_t, __simd128_uint16_t, __simd128_uint16_t, short unsigned int);
__simd128_uint16_t __builtin_mve_vmulltq_poly_pv16qi(__simd128_uint8_t, __simd128_uint8_t);
__simd128_uint32_t __builtin_mve_vmulltq_poly_pv8hi(__simd128_uint16_t, __simd128_uint16_t);
__simd128_float32_t __builtin_mve_vmulq_fv4sf(__simd128_float32_t, __simd128_float32_t);
__simd128_float16_t __builtin_mve_vmulq_fv8hf(__simd128_float16_t, __simd128_float16_t);
__simd128_float32_t __builtin_mve_vmulq_m_fv4sf(__simd128_float32_t, __simd128_float32_t, __simd128_float32_t, short unsigned int);
__simd128_float16_t __builtin_mve_vmulq_m_fv8hf(__simd128_float16_t, __simd128_float16_t, __simd128_float16_t, short unsigned int);
__simd128_float32_t __builtin_mve_vmulq_m_n_fv4sf(__simd128_float32_t, __simd128_float32_t, float, short unsigned int);
__simd128_float16_t __builtin_mve_vmulq_m_n_fv8hf(__simd128_float16_t, __simd128_float16_t, __bf16, short unsigned int);
__simd128_int8_t __builtin_mve_vmulq_m_n_sv16qi(__simd128_int8_t, __simd128_int8_t, signed char, short unsigned int);
__simd128_int32_t __builtin_mve_vmulq_m_n_sv4si(__simd128_int32_t, __simd128_int32_t, int, short unsigned int);
__simd128_int16_t __builtin_mve_vmulq_m_n_sv8hi(__simd128_int16_t, __simd128_int16_t, short int, short unsigned int);
__simd128_uint8_t __builtin_mve_vmulq_m_n_uv16qi(__simd128_uint8_t, __simd128_uint8_t, unsigned char, short unsigned int);
__simd128_uint32_t __builtin_mve_vmulq_m_n_uv4si(__simd128_uint32_t, __simd128_uint32_t, unsigned int, short unsigned int);
__simd128_uint16_t __builtin_mve_vmulq_m_n_uv8hi(__simd128_uint16_t, __simd128_uint16_t, short unsigned int, short unsigned int);
__simd128_int8_t __builtin_mve_vmulq_m_sv16qi(__simd128_int8_t, __simd128_int8_t, __simd128_int8_t, short unsigned int);
__simd128_int32_t __builtin_mve_vmulq_m_sv4si(__simd128_int32_t, __simd128_int32_t, __simd128_int32_t, short unsigned int);
__simd128_int16_t __builtin_mve_vmulq_m_sv8hi(__simd128_int16_t, __simd128_int16_t, __simd128_int16_t, short unsigned int);
__simd128_uint8_t __builtin_mve_vmulq_m_uv16qi(__simd128_uint8_t, __simd128_uint8_t, __simd128_uint8_t, short unsigned int);
__simd128_uint32_t __builtin_mve_vmulq_m_uv4si(__simd128_uint32_t, __simd128_uint32_t, __simd128_uint32_t, short unsigned int);
__simd128_uint16_t __builtin_mve_vmulq_m_uv8hi(__simd128_uint16_t, __simd128_uint16_t, __simd128_uint16_t, short unsigned int);
__simd128_float32_t __builtin_mve_vmulq_n_fv4sf(__simd128_float32_t, float);
__simd128_float16_t __builtin_mve_vmulq_n_fv8hf(__simd128_float16_t, __bf16);
__simd128_int8_t __builtin_mve_vmulq_n_sv16qi(__simd128_int8_t, signed char);
__simd128_int32_t __builtin_mve_vmulq_n_sv4si(__simd128_int32_t, int);
__simd128_int16_t __builtin_mve_vmulq_n_sv8hi(__simd128_int16_t, short int);
__simd128_uint8_t __builtin_mve_vmulq_n_uv16qi(__simd128_uint8_t, unsigned char);
__simd128_uint32_t __builtin_mve_vmulq_n_uv4si(__simd128_uint32_t, unsigned int);
__simd128_uint16_t __builtin_mve_vmulq_n_uv8hi(__simd128_uint16_t, short unsigned int);
__simd128_int8_t __builtin_mve_vmulq_sv16qi(__simd128_int8_t, __simd128_int8_t);
__simd128_int32_t __builtin_mve_vmulq_sv4si(__simd128_int32_t, __simd128_int32_t);
__simd128_int16_t __builtin_mve_vmulq_sv8hi(__simd128_int16_t, __simd128_int16_t);
__simd128_uint8_t __builtin_mve_vmulq_uv16qi(__simd128_uint8_t, __simd128_uint8_t);
__simd128_uint32_t __builtin_mve_vmulq_uv4si(__simd128_uint32_t, __simd128_uint32_t);
__simd128_uint16_t __builtin_mve_vmulq_uv8hi(__simd128_uint16_t, __simd128_uint16_t);
__simd128_int32_t __builtin_mve_vmvnq_m_n_sv4si(__simd128_int32_t, int, short unsigned int);
__simd128_int16_t __builtin_mve_vmvnq_m_n_sv8hi(__simd128_int16_t, int, short unsigned int);
__simd128_uint32_t __builtin_mve_vmvnq_m_n_uv4si(__simd128_uint32_t, int, short unsigned int);
__simd128_uint16_t __builtin_mve_vmvnq_m_n_uv8hi(__simd128_uint16_t, int, short unsigned int);
__simd128_int8_t __builtin_mve_vmvnq_m_sv16qi(__simd128_int8_t, __simd128_int8_t, short unsigned int);
__simd128_int32_t __builtin_mve_vmvnq_m_sv4si(__simd128_int32_t, __simd128_int32_t, short unsigned int);
__simd128_int16_t __builtin_mve_vmvnq_m_sv8hi(__simd128_int16_t, __simd128_int16_t, short unsigned int);
__simd128_uint8_t __builtin_mve_vmvnq_m_uv16qi(__simd128_uint8_t, __simd128_uint8_t, short unsigned int);
__simd128_uint32_t __builtin_mve_vmvnq_m_uv4si(__simd128_uint32_t, __simd128_uint32_t, short unsigned int);
__simd128_uint16_t __builtin_mve_vmvnq_m_uv8hi(__simd128_uint16_t, __simd128_uint16_t, short unsigned int);
__simd128_int32_t __builtin_mve_vmvnq_n_sv4si(short int);
__simd128_int16_t __builtin_mve_vmvnq_n_sv8hi(short int);
__simd128_uint32_t __builtin_mve_vmvnq_n_uv4si(short int);
__simd128_uint16_t __builtin_mve_vmvnq_n_uv8hi(short int);
__simd128_int8_t __builtin_mve_vmvnq_sv16qi(__simd128_int8_t);
__simd128_int32_t __builtin_mve_vmvnq_sv4si(__simd128_int32_t);
__simd128_int16_t __builtin_mve_vmvnq_sv8hi(__simd128_int16_t);
__simd128_uint8_t __builtin_mve_vmvnq_uv16qi(__simd128_uint8_t);
__simd128_uint32_t __builtin_mve_vmvnq_uv4si(__simd128_uint32_t);
__simd128_uint16_t __builtin_mve_vmvnq_uv8hi(__simd128_uint16_t);
__simd128_float32_t __builtin_mve_vnegq_fv4sf(__simd128_float32_t);
__simd128_float16_t __builtin_mve_vnegq_fv8hf(__simd128_float16_t);
__simd128_float32_t __builtin_mve_vnegq_m_fv4sf(__simd128_float32_t, __simd128_float32_t, short unsigned int);
__simd128_float16_t __builtin_mve_vnegq_m_fv8hf(__simd128_float16_t, __simd128_float16_t, short unsigned int);
__simd128_int8_t __builtin_mve_vnegq_m_sv16qi(__simd128_int8_t, __simd128_int8_t, short unsigned int);
__simd128_int32_t __builtin_mve_vnegq_m_sv4si(__simd128_int32_t, __simd128_int32_t, short unsigned int);
__simd128_int16_t __builtin_mve_vnegq_m_sv8hi(__simd128_int16_t, __simd128_int16_t, short unsigned int);
__simd128_int8_t __builtin_mve_vnegq_sv16qi(__simd128_int8_t);
__simd128_int32_t __builtin_mve_vnegq_sv4si(__simd128_int32_t);
__simd128_int16_t __builtin_mve_vnegq_sv8hi(__simd128_int16_t);
__simd128_float32_t __builtin_mve_vornq_fv4sf(__simd128_float32_t, __simd128_float32_t);
__simd128_float16_t __builtin_mve_vornq_fv8hf(__simd128_float16_t, __simd128_float16_t);
__simd128_float32_t __builtin_mve_vornq_m_fv4sf(__simd128_float32_t, __simd128_float32_t, __simd128_float32_t, short unsigned int);
__simd128_float16_t __builtin_mve_vornq_m_fv8hf(__simd128_float16_t, __simd128_float16_t, __simd128_float16_t, short unsigned int);
__simd128_int8_t __builtin_mve_vornq_m_sv16qi(__simd128_int8_t, __simd128_int8_t, __simd128_int8_t, short unsigned int);
__simd128_int32_t __builtin_mve_vornq_m_sv4si(__simd128_int32_t, __simd128_int32_t, __simd128_int32_t, short unsigned int);
__simd128_int16_t __builtin_mve_vornq_m_sv8hi(__simd128_int16_t, __simd128_int16_t, __simd128_int16_t, short unsigned int);
__simd128_uint8_t __builtin_mve_vornq_m_uv16qi(__simd128_uint8_t, __simd128_uint8_t, __simd128_uint8_t, short unsigned int);
__simd128_uint32_t __builtin_mve_vornq_m_uv4si(__simd128_uint32_t, __simd128_uint32_t, __simd128_uint32_t, short unsigned int);
__simd128_uint16_t __builtin_mve_vornq_m_uv8hi(__simd128_uint16_t, __simd128_uint16_t, __simd128_uint16_t, short unsigned int);
__simd128_int8_t __builtin_mve_vornq_sv16qi(__simd128_int8_t, __simd128_int8_t);
__simd128_int32_t __builtin_mve_vornq_sv4si(__simd128_int32_t, __simd128_int32_t);
__simd128_int16_t __builtin_mve_vornq_sv8hi(__simd128_int16_t, __simd128_int16_t);
__simd128_uint8_t __builtin_mve_vornq_uv16qi(__simd128_uint8_t, __simd128_uint8_t);
__simd128_uint32_t __builtin_mve_vornq_uv4si(__simd128_uint32_t, __simd128_uint32_t);
__simd128_uint16_t __builtin_mve_vornq_uv8hi(__simd128_uint16_t, __simd128_uint16_t);
__simd128_float32_t __builtin_mve_vorrq_fv4sf(__simd128_float32_t, __simd128_float32_t);
__simd128_float16_t __builtin_mve_vorrq_fv8hf(__simd128_float16_t, __simd128_float16_t);
__simd128_float32_t __builtin_mve_vorrq_m_fv4sf(__simd128_float32_t, __simd128_float32_t, __simd128_float32_t, short unsigned int);
__simd128_float16_t __builtin_mve_vorrq_m_fv8hf(__simd128_float16_t, __simd128_float16_t, __simd128_float16_t, short unsigned int);
__simd128_int32_t __builtin_mve_vorrq_m_n_sv4si(__simd128_int32_t, int, short unsigned int);
__simd128_int16_t __builtin_mve_vorrq_m_n_sv8hi(__simd128_int16_t, int, short unsigned int);
__simd128_uint32_t __builtin_mve_vorrq_m_n_uv4si(__simd128_uint32_t, int, short unsigned int);
__simd128_uint16_t __builtin_mve_vorrq_m_n_uv8hi(__simd128_uint16_t, int, short unsigned int);
__simd128_int8_t __builtin_mve_vorrq_m_sv16qi(__simd128_int8_t, __simd128_int8_t, __simd128_int8_t, short unsigned int);
__simd128_int32_t __builtin_mve_vorrq_m_sv4si(__simd128_int32_t, __simd128_int32_t, __simd128_int32_t, short unsigned int);
__simd128_int16_t __builtin_mve_vorrq_m_sv8hi(__simd128_int16_t, __simd128_int16_t, __simd128_int16_t, short unsigned int);
__simd128_uint8_t __builtin_mve_vorrq_m_uv16qi(__simd128_uint8_t, __simd128_uint8_t, __simd128_uint8_t, short unsigned int);
__simd128_uint32_t __builtin_mve_vorrq_m_uv4si(__simd128_uint32_t, __simd128_uint32_t, __simd128_uint32_t, short unsigned int);
__simd128_uint16_t __builtin_mve_vorrq_m_uv8hi(__simd128_uint16_t, __simd128_uint16_t, __simd128_uint16_t, short unsigned int);
__simd128_int32_t __builtin_mve_vorrq_n_sv4si(__simd128_int32_t, int);
__simd128_int16_t __builtin_mve_vorrq_n_sv8hi(__simd128_int16_t, int);
__simd128_uint32_t __builtin_mve_vorrq_n_uv4si(__simd128_uint32_t, int);
__simd128_uint16_t __builtin_mve_vorrq_n_uv8hi(__simd128_uint16_t, int);
__simd128_int8_t __builtin_mve_vorrq_sv16qi(__simd128_int8_t, __simd128_int8_t);
__simd128_int32_t __builtin_mve_vorrq_sv4si(__simd128_int32_t, __simd128_int32_t);
__simd128_int16_t __builtin_mve_vorrq_sv8hi(__simd128_int16_t, __simd128_int16_t);
__simd128_uint8_t __builtin_mve_vorrq_uv16qi(__simd128_uint8_t, __simd128_uint8_t);
__simd128_uint32_t __builtin_mve_vorrq_uv4si(__simd128_uint32_t, __simd128_uint32_t);
__simd128_uint16_t __builtin_mve_vorrq_uv8hi(__simd128_uint16_t, __simd128_uint16_t);
short unsigned int __builtin_mve_vpnothi(short unsigned int);
__simd128_float32_t __builtin_mve_vpselq_fv4sf(__simd128_float32_t, __simd128_float32_t, short unsigned int);
__simd128_float16_t __builtin_mve_vpselq_fv8hf(__simd128_float16_t, __simd128_float16_t, short unsigned int);
__simd128_int8_t __builtin_mve_vpselq_sv16qi(__simd128_int8_t, __simd128_int8_t, short unsigned int);
__simd128_int64_t __builtin_mve_vpselq_sv2di(__simd128_int64_t, __simd128_int64_t, short unsigned int);
__simd128_int32_t __builtin_mve_vpselq_sv4si(__simd128_int32_t, __simd128_int32_t, short unsigned int);
__simd128_int16_t __builtin_mve_vpselq_sv8hi(__simd128_int16_t, __simd128_int16_t, short unsigned int);
__simd128_uint8_t __builtin_mve_vpselq_uv16qi(__simd128_uint8_t, __simd128_uint8_t, short unsigned int);
__simd128_uint64_t __builtin_mve_vpselq_uv2di(__simd128_uint64_t, __simd128_uint64_t, short unsigned int);
__simd128_uint32_t __builtin_mve_vpselq_uv4si(__simd128_uint32_t, __simd128_uint32_t, short unsigned int);
__simd128_uint16_t __builtin_mve_vpselq_uv8hi(__simd128_uint16_t, __simd128_uint16_t, short unsigned int);
__simd128_int8_t __builtin_mve_vqabsq_m_sv16qi(__simd128_int8_t, __simd128_int8_t, short unsigned int);
__simd128_int32_t __builtin_mve_vqabsq_m_sv4si(__simd128_int32_t, __simd128_int32_t, short unsigned int);
__simd128_int16_t __builtin_mve_vqabsq_m_sv8hi(__simd128_int16_t, __simd128_int16_t, short unsigned int);
__simd128_int8_t __builtin_mve_vqabsq_sv16qi(__simd128_int8_t);
__simd128_int32_t __builtin_mve_vqabsq_sv4si(__simd128_int32_t);
__simd128_int16_t __builtin_mve_vqabsq_sv8hi(__simd128_int16_t);
__simd128_int8_t __builtin_mve_vqaddq_m_n_sv16qi(__simd128_int8_t, __simd128_int8_t, signed char, short unsigned int);
__simd128_int32_t __builtin_mve_vqaddq_m_n_sv4si(__simd128_int32_t, __simd128_int32_t, int, short unsigned int);
__simd128_int16_t __builtin_mve_vqaddq_m_n_sv8hi(__simd128_int16_t, __simd128_int16_t, short int, short unsigned int);
__simd128_uint8_t __builtin_mve_vqaddq_m_n_uv16qi(__simd128_uint8_t, __simd128_uint8_t, unsigned char, short unsigned int);
__simd128_uint32_t __builtin_mve_vqaddq_m_n_uv4si(__simd128_uint32_t, __simd128_uint32_t, unsigned int, short unsigned int);
__simd128_uint16_t __builtin_mve_vqaddq_m_n_uv8hi(__simd128_uint16_t, __simd128_uint16_t, short unsigned int, short unsigned int);
__simd128_int8_t __builtin_mve_vqaddq_m_sv16qi(__simd128_int8_t, __simd128_int8_t, __simd128_int8_t, short unsigned int);
__simd128_int32_t __builtin_mve_vqaddq_m_sv4si(__simd128_int32_t, __simd128_int32_t, __simd128_int32_t, short unsigned int);
__simd128_int16_t __builtin_mve_vqaddq_m_sv8hi(__simd128_int16_t, __simd128_int16_t, __simd128_int16_t, short unsigned int);
__simd128_uint8_t __builtin_mve_vqaddq_m_uv16qi(__simd128_uint8_t, __simd128_uint8_t, __simd128_uint8_t, short unsigned int);
__simd128_uint32_t __builtin_mve_vqaddq_m_uv4si(__simd128_uint32_t, __simd128_uint32_t, __simd128_uint32_t, short unsigned int);
__simd128_uint16_t __builtin_mve_vqaddq_m_uv8hi(__simd128_uint16_t, __simd128_uint16_t, __simd128_uint16_t, short unsigned int);
__simd128_int8_t __builtin_mve_vqaddq_n_sv16qi(__simd128_int8_t, signed char);
__simd128_int32_t __builtin_mve_vqaddq_n_sv4si(__simd128_int32_t, int);
__simd128_int16_t __builtin_mve_vqaddq_n_sv8hi(__simd128_int16_t, short int);
__simd128_uint8_t __builtin_mve_vqaddq_n_uv16qi(__simd128_uint8_t, unsigned char);
__simd128_uint32_t __builtin_mve_vqaddq_n_uv4si(__simd128_uint32_t, unsigned int);
__simd128_uint16_t __builtin_mve_vqaddq_n_uv8hi(__simd128_uint16_t, short unsigned int);
__simd128_int8_t __builtin_mve_vqaddq_sv16qi(__simd128_int8_t, __simd128_int8_t);
__simd128_int32_t __builtin_mve_vqaddq_sv4si(__simd128_int32_t, __simd128_int32_t);
__simd128_int16_t __builtin_mve_vqaddq_sv8hi(__simd128_int16_t, __simd128_int16_t);
__simd128_uint8_t __builtin_mve_vqaddq_uv16qi(__simd128_uint8_t, __simd128_uint8_t);
__simd128_uint32_t __builtin_mve_vqaddq_uv4si(__simd128_uint32_t, __simd128_uint32_t);
__simd128_uint16_t __builtin_mve_vqaddq_uv8hi(__simd128_uint16_t, __simd128_uint16_t);
__simd128_int8_t __builtin_mve_vqdmladhq_m_sv16qi(__simd128_int8_t, __simd128_int8_t, __simd128_int8_t, short unsigned int);
__simd128_int32_t __builtin_mve_vqdmladhq_m_sv4si(__simd128_int32_t, __simd128_int32_t, __simd128_int32_t, short unsigned int);
__simd128_int16_t __builtin_mve_vqdmladhq_m_sv8hi(__simd128_int16_t, __simd128_int16_t, __simd128_int16_t, short unsigned int);
__simd128_int8_t __builtin_mve_vqdmladhq_sv16qi(__simd128_int8_t, __simd128_int8_t, __simd128_int8_t);
__simd128_int32_t __builtin_mve_vqdmladhq_sv4si(__simd128_int32_t, __simd128_int32_t, __simd128_int32_t);
__simd128_int16_t __builtin_mve_vqdmladhq_sv8hi(__simd128_int16_t, __simd128_int16_t, __simd128_int16_t);
__simd128_int8_t __builtin_mve_vqdmladhxq_m_sv16qi(__simd128_int8_t, __simd128_int8_t, __simd128_int8_t, short unsigned int);
__simd128_int32_t __builtin_mve_vqdmladhxq_m_sv4si(__simd128_int32_t, __simd128_int32_t, __simd128_int32_t, short unsigned int);
__simd128_int16_t __builtin_mve_vqdmladhxq_m_sv8hi(__simd128_int16_t, __simd128_int16_t, __simd128_int16_t, short unsigned int);
__simd128_int8_t __builtin_mve_vqdmladhxq_sv16qi(__simd128_int8_t, __simd128_int8_t, __simd128_int8_t);
__simd128_int32_t __builtin_mve_vqdmladhxq_sv4si(__simd128_int32_t, __simd128_int32_t, __simd128_int32_t);
__simd128_int16_t __builtin_mve_vqdmladhxq_sv8hi(__simd128_int16_t, __simd128_int16_t, __simd128_int16_t);
__simd128_int8_t __builtin_mve_vqdmlahq_m_n_sv16qi(__simd128_int8_t, __simd128_int8_t, signed char, short unsigned int);
__simd128_int32_t __builtin_mve_vqdmlahq_m_n_sv4si(__simd128_int32_t, __simd128_int32_t, int, short unsigned int);
__simd128_int16_t __builtin_mve_vqdmlahq_m_n_sv8hi(__simd128_int16_t, __simd128_int16_t, short int, short unsigned int);
__simd128_int8_t __builtin_mve_vqdmlahq_n_sv16qi(__simd128_int8_t, __simd128_int8_t, signed char);
__simd128_int32_t __builtin_mve_vqdmlahq_n_sv4si(__simd128_int32_t, __simd128_int32_t, int);
__simd128_int16_t __builtin_mve_vqdmlahq_n_sv8hi(__simd128_int16_t, __simd128_int16_t, short int);
__simd128_uint8_t __builtin_mve_vqdmlahq_n_uv16qi(__simd128_uint8_t, __simd128_uint8_t, unsigned char);
__simd128_uint32_t __builtin_mve_vqdmlahq_n_uv4si(__simd128_uint32_t, __simd128_uint32_t, unsigned int);
__simd128_uint16_t __builtin_mve_vqdmlahq_n_uv8hi(__simd128_uint16_t, __simd128_uint16_t, short unsigned int);
__simd128_int8_t __builtin_mve_vqdmlsdhq_m_sv16qi(__simd128_int8_t, __simd128_int8_t, __simd128_int8_t, short unsigned int);
__simd128_int32_t __builtin_mve_vqdmlsdhq_m_sv4si(__simd128_int32_t, __simd128_int32_t, __simd128_int32_t, short unsigned int);
__simd128_int16_t __builtin_mve_vqdmlsdhq_m_sv8hi(__simd128_int16_t, __simd128_int16_t, __simd128_int16_t, short unsigned int);
__simd128_int8_t __builtin_mve_vqdmlsdhq_sv16qi(__simd128_int8_t, __simd128_int8_t, __simd128_int8_t);
__simd128_int32_t __builtin_mve_vqdmlsdhq_sv4si(__simd128_int32_t, __simd128_int32_t, __simd128_int32_t);
__simd128_int16_t __builtin_mve_vqdmlsdhq_sv8hi(__simd128_int16_t, __simd128_int16_t, __simd128_int16_t);
__simd128_int8_t __builtin_mve_vqdmlsdhxq_m_sv16qi(__simd128_int8_t, __simd128_int8_t, __simd128_int8_t, short unsigned int);
__simd128_int32_t __builtin_mve_vqdmlsdhxq_m_sv4si(__simd128_int32_t, __simd128_int32_t, __simd128_int32_t, short unsigned int);
__simd128_int16_t __builtin_mve_vqdmlsdhxq_m_sv8hi(__simd128_int16_t, __simd128_int16_t, __simd128_int16_t, short unsigned int);
__simd128_int8_t __builtin_mve_vqdmlsdhxq_sv16qi(__simd128_int8_t, __simd128_int8_t, __simd128_int8_t);
__simd128_int32_t __builtin_mve_vqdmlsdhxq_sv4si(__simd128_int32_t, __simd128_int32_t, __simd128_int32_t);
__simd128_int16_t __builtin_mve_vqdmlsdhxq_sv8hi(__simd128_int16_t, __simd128_int16_t, __simd128_int16_t);
__simd128_int8_t __builtin_mve_vqdmulhq_m_n_sv16qi(__simd128_int8_t, __simd128_int8_t, signed char, short unsigned int);
__simd128_int32_t __builtin_mve_vqdmulhq_m_n_sv4si(__simd128_int32_t, __simd128_int32_t, int, short unsigned int);
__simd128_int16_t __builtin_mve_vqdmulhq_m_n_sv8hi(__simd128_int16_t, __simd128_int16_t, short int, short unsigned int);
__simd128_int8_t __builtin_mve_vqdmulhq_m_sv16qi(__simd128_int8_t, __simd128_int8_t, __simd128_int8_t, short unsigned int);
__simd128_int32_t __builtin_mve_vqdmulhq_m_sv4si(__simd128_int32_t, __simd128_int32_t, __simd128_int32_t, short unsigned int);
__simd128_int16_t __builtin_mve_vqdmulhq_m_sv8hi(__simd128_int16_t, __simd128_int16_t, __simd128_int16_t, short unsigned int);
__simd128_int8_t __builtin_mve_vqdmulhq_n_sv16qi(__simd128_int8_t, signed char);
__simd128_int32_t __builtin_mve_vqdmulhq_n_sv4si(__simd128_int32_t, int);
__simd128_int16_t __builtin_mve_vqdmulhq_n_sv8hi(__simd128_int16_t, short int);
__simd128_int8_t __builtin_mve_vqdmulhq_sv16qi(__simd128_int8_t, __simd128_int8_t);
__simd128_int32_t __builtin_mve_vqdmulhq_sv4si(__simd128_int32_t, __simd128_int32_t);
__simd128_int16_t __builtin_mve_vqdmulhq_sv8hi(__simd128_int16_t, __simd128_int16_t);
__simd128_int64_t __builtin_mve_vqdmullbq_m_n_sv4si(__simd128_int64_t, __simd128_int32_t, int, short unsigned int);
__simd128_int32_t __builtin_mve_vqdmullbq_m_n_sv8hi(__simd128_int32_t, __simd128_int16_t, short int, short unsigned int);
__simd128_int64_t __builtin_mve_vqdmullbq_m_sv4si(__simd128_int64_t, __simd128_int32_t, __simd128_int32_t, short unsigned int);
__simd128_int32_t __builtin_mve_vqdmullbq_m_sv8hi(__simd128_int32_t, __simd128_int16_t, __simd128_int16_t, short unsigned int);
__simd128_int64_t __builtin_mve_vqdmullbq_n_sv4si(__simd128_int32_t, int);
__simd128_int32_t __builtin_mve_vqdmullbq_n_sv8hi(__simd128_int16_t, short int);
__simd128_int64_t __builtin_mve_vqdmullbq_sv4si(__simd128_int32_t, __simd128_int32_t);
__simd128_int32_t __builtin_mve_vqdmullbq_sv8hi(__simd128_int16_t, __simd128_int16_t);
__simd128_int64_t __builtin_mve_vqdmulltq_m_n_sv4si(__simd128_int64_t, __simd128_int32_t, int, short unsigned int);
__simd128_int32_t __builtin_mve_vqdmulltq_m_n_sv8hi(__simd128_int32_t, __simd128_int16_t, short int, short unsigned int);
__simd128_int64_t __builtin_mve_vqdmulltq_m_sv4si(__simd128_int64_t, __simd128_int32_t, __simd128_int32_t, short unsigned int);
__simd128_int32_t __builtin_mve_vqdmulltq_m_sv8hi(__simd128_int32_t, __simd128_int16_t, __simd128_int16_t, short unsigned int);
__simd128_int64_t __builtin_mve_vqdmulltq_n_sv4si(__simd128_int32_t, int);
__simd128_int32_t __builtin_mve_vqdmulltq_n_sv8hi(__simd128_int16_t, short int);
__simd128_int64_t __builtin_mve_vqdmulltq_sv4si(__simd128_int32_t, __simd128_int32_t);
__simd128_int32_t __builtin_mve_vqdmulltq_sv8hi(__simd128_int16_t, __simd128_int16_t);
__simd128_int16_t __builtin_mve_vqmovnbq_m_sv4si(__simd128_int16_t, __simd128_int32_t, short unsigned int);
__simd128_int8_t __builtin_mve_vqmovnbq_m_sv8hi(__simd128_int8_t, __simd128_int16_t, short unsigned int);
__simd128_uint16_t __builtin_mve_vqmovnbq_m_uv4si(__simd128_uint16_t, __simd128_uint32_t, short unsigned int);
__simd128_uint8_t __builtin_mve_vqmovnbq_m_uv8hi(__simd128_uint8_t, __simd128_uint16_t, short unsigned int);
__simd128_int16_t __builtin_mve_vqmovnbq_sv4si(__simd128_int16_t, __simd128_int32_t);
__simd128_int8_t __builtin_mve_vqmovnbq_sv8hi(__simd128_int8_t, __simd128_int16_t);
__simd128_uint16_t __builtin_mve_vqmovnbq_uv4si(__simd128_uint16_t, __simd128_uint32_t);
__simd128_uint8_t __builtin_mve_vqmovnbq_uv8hi(__simd128_uint8_t, __simd128_uint16_t);
__simd128_int16_t __builtin_mve_vqmovntq_m_sv4si(__simd128_int16_t, __simd128_int32_t, short unsigned int);
__simd128_int8_t __builtin_mve_vqmovntq_m_sv8hi(__simd128_int8_t, __simd128_int16_t, short unsigned int);
__simd128_uint16_t __builtin_mve_vqmovntq_m_uv4si(__simd128_uint16_t, __simd128_uint32_t, short unsigned int);
__simd128_uint8_t __builtin_mve_vqmovntq_m_uv8hi(__simd128_uint8_t, __simd128_uint16_t, short unsigned int);
__simd128_int16_t __builtin_mve_vqmovntq_sv4si(__simd128_int16_t, __simd128_int32_t);
__simd128_int8_t __builtin_mve_vqmovntq_sv8hi(__simd128_int8_t, __simd128_int16_t);
__simd128_uint16_t __builtin_mve_vqmovntq_uv4si(__simd128_uint16_t, __simd128_uint32_t);
__simd128_uint8_t __builtin_mve_vqmovntq_uv8hi(__simd128_uint8_t, __simd128_uint16_t);
__simd128_uint16_t __builtin_mve_vqmovunbq_m_sv4si(__simd128_uint16_t, __simd128_int32_t, short unsigned int);
__simd128_uint8_t __builtin_mve_vqmovunbq_m_sv8hi(__simd128_uint8_t, __simd128_int16_t, short unsigned int);
__simd128_uint16_t __builtin_mve_vqmovunbq_sv4si(__simd128_uint16_t, __simd128_int32_t);
__simd128_uint8_t __builtin_mve_vqmovunbq_sv8hi(__simd128_uint8_t, __simd128_int16_t);
__simd128_uint16_t __builtin_mve_vqmovuntq_m_sv4si(__simd128_uint16_t, __simd128_int32_t, short unsigned int);
__simd128_uint8_t __builtin_mve_vqmovuntq_m_sv8hi(__simd128_uint8_t, __simd128_int16_t, short unsigned int);
__simd128_uint16_t __builtin_mve_vqmovuntq_sv4si(__simd128_uint16_t, __simd128_int32_t);
__simd128_uint8_t __builtin_mve_vqmovuntq_sv8hi(__simd128_uint8_t, __simd128_int16_t);
__simd128_int8_t __builtin_mve_vqnegq_m_sv16qi(__simd128_int8_t, __simd128_int8_t, short unsigned int);
__simd128_int32_t __builtin_mve_vqnegq_m_sv4si(__simd128_int32_t, __simd128_int32_t, short unsigned int);
__simd128_int16_t __builtin_mve_vqnegq_m_sv8hi(__simd128_int16_t, __simd128_int16_t, short unsigned int);
__simd128_int8_t __builtin_mve_vqnegq_sv16qi(__simd128_int8_t);
__simd128_int32_t __builtin_mve_vqnegq_sv4si(__simd128_int32_t);
__simd128_int16_t __builtin_mve_vqnegq_sv8hi(__simd128_int16_t);
__simd128_int8_t __builtin_mve_vqrdmladhq_m_sv16qi(__simd128_int8_t, __simd128_int8_t, __simd128_int8_t, short unsigned int);
__simd128_int32_t __builtin_mve_vqrdmladhq_m_sv4si(__simd128_int32_t, __simd128_int32_t, __simd128_int32_t, short unsigned int);
__simd128_int16_t __builtin_mve_vqrdmladhq_m_sv8hi(__simd128_int16_t, __simd128_int16_t, __simd128_int16_t, short unsigned int);
__simd128_int8_t __builtin_mve_vqrdmladhq_sv16qi(__simd128_int8_t, __simd128_int8_t, __simd128_int8_t);
__simd128_int32_t __builtin_mve_vqrdmladhq_sv4si(__simd128_int32_t, __simd128_int32_t, __simd128_int32_t);
__simd128_int16_t __builtin_mve_vqrdmladhq_sv8hi(__simd128_int16_t, __simd128_int16_t, __simd128_int16_t);
__simd128_int8_t __builtin_mve_vqrdmladhxq_m_sv16qi(__simd128_int8_t, __simd128_int8_t, __simd128_int8_t, short unsigned int);
__simd128_int32_t __builtin_mve_vqrdmladhxq_m_sv4si(__simd128_int32_t, __simd128_int32_t, __simd128_int32_t, short unsigned int);
__simd128_int16_t __builtin_mve_vqrdmladhxq_m_sv8hi(__simd128_int16_t, __simd128_int16_t, __simd128_int16_t, short unsigned int);
__simd128_int8_t __builtin_mve_vqrdmladhxq_sv16qi(__simd128_int8_t, __simd128_int8_t, __simd128_int8_t);
__simd128_int32_t __builtin_mve_vqrdmladhxq_sv4si(__simd128_int32_t, __simd128_int32_t, __simd128_int32_t);
__simd128_int16_t __builtin_mve_vqrdmladhxq_sv8hi(__simd128_int16_t, __simd128_int16_t, __simd128_int16_t);
__simd128_int8_t __builtin_mve_vqrdmlahq_m_n_sv16qi(__simd128_int8_t, __simd128_int8_t, signed char, short unsigned int);
__simd128_int32_t __builtin_mve_vqrdmlahq_m_n_sv4si(__simd128_int32_t, __simd128_int32_t, int, short unsigned int);
__simd128_int16_t __builtin_mve_vqrdmlahq_m_n_sv8hi(__simd128_int16_t, __simd128_int16_t, short int, short unsigned int);
__simd128_int8_t __builtin_mve_vqrdmlahq_n_sv16qi(__simd128_int8_t, __simd128_int8_t, signed char);
__simd128_int32_t __builtin_mve_vqrdmlahq_n_sv4si(__simd128_int32_t, __simd128_int32_t, int);
__simd128_int16_t __builtin_mve_vqrdmlahq_n_sv8hi(__simd128_int16_t, __simd128_int16_t, short int);
__simd128_uint8_t __builtin_mve_vqrdmlahq_n_uv16qi(__simd128_uint8_t, __simd128_uint8_t, unsigned char);
__simd128_uint32_t __builtin_mve_vqrdmlahq_n_uv4si(__simd128_uint32_t, __simd128_uint32_t, unsigned int);
__simd128_uint16_t __builtin_mve_vqrdmlahq_n_uv8hi(__simd128_uint16_t, __simd128_uint16_t, short unsigned int);
__simd128_int8_t __builtin_mve_vqrdmlashq_m_n_sv16qi(__simd128_int8_t, __simd128_int8_t, signed char, short unsigned int);
__simd128_int32_t __builtin_mve_vqrdmlashq_m_n_sv4si(__simd128_int32_t, __simd128_int32_t, int, short unsigned int);
__simd128_int16_t __builtin_mve_vqrdmlashq_m_n_sv8hi(__simd128_int16_t, __simd128_int16_t, short int, short unsigned int);
__simd128_int8_t __builtin_mve_vqrdmlashq_n_sv16qi(__simd128_int8_t, __simd128_int8_t, signed char);
__simd128_int32_t __builtin_mve_vqrdmlashq_n_sv4si(__simd128_int32_t, __simd128_int32_t, int);
__simd128_int16_t __builtin_mve_vqrdmlashq_n_sv8hi(__simd128_int16_t, __simd128_int16_t, short int);
__simd128_uint8_t __builtin_mve_vqrdmlashq_n_uv16qi(__simd128_uint8_t, __simd128_uint8_t, unsigned char);
__simd128_uint32_t __builtin_mve_vqrdmlashq_n_uv4si(__simd128_uint32_t, __simd128_uint32_t, unsigned int);
__simd128_uint16_t __builtin_mve_vqrdmlashq_n_uv8hi(__simd128_uint16_t, __simd128_uint16_t, short unsigned int);
__simd128_int8_t __builtin_mve_vqrdmlsdhq_m_sv16qi(__simd128_int8_t, __simd128_int8_t, __simd128_int8_t, short unsigned int);
__simd128_int32_t __builtin_mve_vqrdmlsdhq_m_sv4si(__simd128_int32_t, __simd128_int32_t, __simd128_int32_t, short unsigned int);
__simd128_int16_t __builtin_mve_vqrdmlsdhq_m_sv8hi(__simd128_int16_t, __simd128_int16_t, __simd128_int16_t, short unsigned int);
__simd128_int8_t __builtin_mve_vqrdmlsdhq_sv16qi(__simd128_int8_t, __simd128_int8_t, __simd128_int8_t);
__simd128_int32_t __builtin_mve_vqrdmlsdhq_sv4si(__simd128_int32_t, __simd128_int32_t, __simd128_int32_t);
__simd128_int16_t __builtin_mve_vqrdmlsdhq_sv8hi(__simd128_int16_t, __simd128_int16_t, __simd128_int16_t);
__simd128_int8_t __builtin_mve_vqrdmlsdhxq_m_sv16qi(__simd128_int8_t, __simd128_int8_t, __simd128_int8_t, short unsigned int);
__simd128_int32_t __builtin_mve_vqrdmlsdhxq_m_sv4si(__simd128_int32_t, __simd128_int32_t, __simd128_int32_t, short unsigned int);
__simd128_int16_t __builtin_mve_vqrdmlsdhxq_m_sv8hi(__simd128_int16_t, __simd128_int16_t, __simd128_int16_t, short unsigned int);
__simd128_int8_t __builtin_mve_vqrdmlsdhxq_sv16qi(__simd128_int8_t, __simd128_int8_t, __simd128_int8_t);
__simd128_int32_t __builtin_mve_vqrdmlsdhxq_sv4si(__simd128_int32_t, __simd128_int32_t, __simd128_int32_t);
__simd128_int16_t __builtin_mve_vqrdmlsdhxq_sv8hi(__simd128_int16_t, __simd128_int16_t, __simd128_int16_t);
__simd128_int8_t __builtin_mve_vqrdmulhq_m_n_sv16qi(__simd128_int8_t, __simd128_int8_t, signed char, short unsigned int);
__simd128_int32_t __builtin_mve_vqrdmulhq_m_n_sv4si(__simd128_int32_t, __simd128_int32_t, int, short unsigned int);
__simd128_int16_t __builtin_mve_vqrdmulhq_m_n_sv8hi(__simd128_int16_t, __simd128_int16_t, short int, short unsigned int);
__simd128_int8_t __builtin_mve_vqrdmulhq_m_sv16qi(__simd128_int8_t, __simd128_int8_t, __simd128_int8_t, short unsigned int);
__simd128_int32_t __builtin_mve_vqrdmulhq_m_sv4si(__simd128_int32_t, __simd128_int32_t, __simd128_int32_t, short unsigned int);
__simd128_int16_t __builtin_mve_vqrdmulhq_m_sv8hi(__simd128_int16_t, __simd128_int16_t, __simd128_int16_t, short unsigned int);
__simd128_int8_t __builtin_mve_vqrdmulhq_n_sv16qi(__simd128_int8_t, signed char);
__simd128_int32_t __builtin_mve_vqrdmulhq_n_sv4si(__simd128_int32_t, int);
__simd128_int16_t __builtin_mve_vqrdmulhq_n_sv8hi(__simd128_int16_t, short int);
__simd128_int8_t __builtin_mve_vqrdmulhq_sv16qi(__simd128_int8_t, __simd128_int8_t);
__simd128_int32_t __builtin_mve_vqrdmulhq_sv4si(__simd128_int32_t, __simd128_int32_t);
__simd128_int16_t __builtin_mve_vqrdmulhq_sv8hi(__simd128_int16_t, __simd128_int16_t);
__simd128_int8_t __builtin_mve_vqrshlq_m_n_sv16qi(__simd128_int8_t, int, short unsigned int);
__simd128_int32_t __builtin_mve_vqrshlq_m_n_sv4si(__simd128_int32_t, int, short unsigned int);
__simd128_int16_t __builtin_mve_vqrshlq_m_n_sv8hi(__simd128_int16_t, int, short unsigned int);
__simd128_uint8_t __builtin_mve_vqrshlq_m_n_uv16qi(__simd128_uint8_t, int, short unsigned int);
__simd128_uint32_t __builtin_mve_vqrshlq_m_n_uv4si(__simd128_uint32_t, int, short unsigned int);
__simd128_uint16_t __builtin_mve_vqrshlq_m_n_uv8hi(__simd128_uint16_t, int, short unsigned int);
__simd128_int8_t __builtin_mve_vqrshlq_m_sv16qi(__simd128_int8_t, __simd128_int8_t, __simd128_int8_t, short unsigned int);
__simd128_int32_t __builtin_mve_vqrshlq_m_sv4si(__simd128_int32_t, __simd128_int32_t, __simd128_int32_t, short unsigned int);
__simd128_int16_t __builtin_mve_vqrshlq_m_sv8hi(__simd128_int16_t, __simd128_int16_t, __simd128_int16_t, short unsigned int);
__simd128_uint8_t __builtin_mve_vqrshlq_m_uv16qi(__simd128_uint8_t, __simd128_uint8_t, __simd128_int8_t, short unsigned int);
__simd128_uint32_t __builtin_mve_vqrshlq_m_uv4si(__simd128_uint32_t, __simd128_uint32_t, __simd128_int32_t, short unsigned int);
__simd128_uint16_t __builtin_mve_vqrshlq_m_uv8hi(__simd128_uint16_t, __simd128_uint16_t, __simd128_int16_t, short unsigned int);
__simd128_int8_t __builtin_mve_vqrshlq_n_sv16qi(__simd128_int8_t, int);
__simd128_int32_t __builtin_mve_vqrshlq_n_sv4si(__simd128_int32_t, int);
__simd128_int16_t __builtin_mve_vqrshlq_n_sv8hi(__simd128_int16_t, int);
__simd128_uint8_t __builtin_mve_vqrshlq_n_uv16qi(__simd128_uint8_t, int);
__simd128_uint32_t __builtin_mve_vqrshlq_n_uv4si(__simd128_uint32_t, int);
__simd128_uint16_t __builtin_mve_vqrshlq_n_uv8hi(__simd128_uint16_t, int);
__simd128_int8_t __builtin_mve_vqrshlq_sv16qi(__simd128_int8_t, __simd128_int8_t);
__simd128_int32_t __builtin_mve_vqrshlq_sv4si(__simd128_int32_t, __simd128_int32_t);
__simd128_int16_t __builtin_mve_vqrshlq_sv8hi(__simd128_int16_t, __simd128_int16_t);
__simd128_uint8_t __builtin_mve_vqrshlq_uv16qi(__simd128_uint8_t, __simd128_int8_t);
__simd128_uint32_t __builtin_mve_vqrshlq_uv4si(__simd128_uint32_t, __simd128_int32_t);
__simd128_uint16_t __builtin_mve_vqrshlq_uv8hi(__simd128_uint16_t, __simd128_int16_t);
__simd128_int16_t __builtin_mve_vqrshrnbq_m_n_sv4si(__simd128_int16_t, __simd128_int32_t, int, short unsigned int);
__simd128_int8_t __builtin_mve_vqrshrnbq_m_n_sv8hi(__simd128_int8_t, __simd128_int16_t, int, short unsigned int);
__simd128_uint16_t __builtin_mve_vqrshrnbq_m_n_uv4si(__simd128_uint16_t, __simd128_uint32_t, int, short unsigned int);
__simd128_uint8_t __builtin_mve_vqrshrnbq_m_n_uv8hi(__simd128_uint8_t, __simd128_uint16_t, int, short unsigned int);
__simd128_int16_t __builtin_mve_vqrshrnbq_n_sv4si(__simd128_int16_t, __simd128_int32_t, int);
__simd128_int8_t __builtin_mve_vqrshrnbq_n_sv8hi(__simd128_int8_t, __simd128_int16_t, int);
__simd128_uint16_t __builtin_mve_vqrshrnbq_n_uv4si(__simd128_uint16_t, __simd128_uint32_t, int);
__simd128_uint8_t __builtin_mve_vqrshrnbq_n_uv8hi(__simd128_uint8_t, __simd128_uint16_t, int);
__simd128_int16_t __builtin_mve_vqrshrntq_m_n_sv4si(__simd128_int16_t, __simd128_int32_t, int, short unsigned int);
__simd128_int8_t __builtin_mve_vqrshrntq_m_n_sv8hi(__simd128_int8_t, __simd128_int16_t, int, short unsigned int);
__simd128_uint16_t __builtin_mve_vqrshrntq_m_n_uv4si(__simd128_uint16_t, __simd128_uint32_t, int, short unsigned int);
__simd128_uint8_t __builtin_mve_vqrshrntq_m_n_uv8hi(__simd128_uint8_t, __simd128_uint16_t, int, short unsigned int);
__simd128_int16_t __builtin_mve_vqrshrntq_n_sv4si(__simd128_int16_t, __simd128_int32_t, int);
__simd128_int8_t __builtin_mve_vqrshrntq_n_sv8hi(__simd128_int8_t, __simd128_int16_t, int);
__simd128_uint16_t __builtin_mve_vqrshrntq_n_uv4si(__simd128_uint16_t, __simd128_uint32_t, int);
__simd128_uint8_t __builtin_mve_vqrshrntq_n_uv8hi(__simd128_uint8_t, __simd128_uint16_t, int);
__simd128_uint16_t __builtin_mve_vqrshrunbq_m_n_sv4si(__simd128_uint16_t, __simd128_int32_t, int, short unsigned int);
__simd128_uint8_t __builtin_mve_vqrshrunbq_m_n_sv8hi(__simd128_uint8_t, __simd128_int16_t, int, short unsigned int);
__simd128_uint16_t __builtin_mve_vqrshrunbq_n_sv4si(__simd128_uint16_t, __simd128_int32_t, int);
__simd128_uint8_t __builtin_mve_vqrshrunbq_n_sv8hi(__simd128_uint8_t, __simd128_int16_t, int);
__simd128_uint16_t __builtin_mve_vqrshruntq_m_n_sv4si(__simd128_uint16_t, __simd128_int32_t, int, short unsigned int);
__simd128_uint8_t __builtin_mve_vqrshruntq_m_n_sv8hi(__simd128_uint8_t, __simd128_int16_t, int, short unsigned int);
__simd128_uint16_t __builtin_mve_vqrshruntq_n_sv4si(__simd128_uint16_t, __simd128_int32_t, int);
__simd128_uint8_t __builtin_mve_vqrshruntq_n_sv8hi(__simd128_uint8_t, __simd128_int16_t, int);
__simd128_int8_t __builtin_mve_vqshlq_m_n_sv16qi(__simd128_int8_t, __simd128_int8_t, int, short unsigned int);
__simd128_int32_t __builtin_mve_vqshlq_m_n_sv4si(__simd128_int32_t, __simd128_int32_t, int, short unsigned int);
__simd128_int16_t __builtin_mve_vqshlq_m_n_sv8hi(__simd128_int16_t, __simd128_int16_t, int, short unsigned int);
__simd128_uint8_t __builtin_mve_vqshlq_m_n_uv16qi(__simd128_uint8_t, __simd128_uint8_t, int, short unsigned int);
__simd128_uint32_t __builtin_mve_vqshlq_m_n_uv4si(__simd128_uint32_t, __simd128_uint32_t, int, short unsigned int);
__simd128_uint16_t __builtin_mve_vqshlq_m_n_uv8hi(__simd128_uint16_t, __simd128_uint16_t, int, short unsigned int);
__simd128_int8_t __builtin_mve_vqshlq_m_r_sv16qi(__simd128_int8_t, int, short unsigned int);
__simd128_int32_t __builtin_mve_vqshlq_m_r_sv4si(__simd128_int32_t, int, short unsigned int);
__simd128_int16_t __builtin_mve_vqshlq_m_r_sv8hi(__simd128_int16_t, int, short unsigned int);
__simd128_uint8_t __builtin_mve_vqshlq_m_r_uv16qi(__simd128_uint8_t, int, short unsigned int);
__simd128_uint32_t __builtin_mve_vqshlq_m_r_uv4si(__simd128_uint32_t, int, short unsigned int);
__simd128_uint16_t __builtin_mve_vqshlq_m_r_uv8hi(__simd128_uint16_t, int, short unsigned int);
__simd128_int8_t __builtin_mve_vqshlq_m_sv16qi(__simd128_int8_t, __simd128_int8_t, __simd128_int8_t, short unsigned int);
__simd128_int32_t __builtin_mve_vqshlq_m_sv4si(__simd128_int32_t, __simd128_int32_t, __simd128_int32_t, short unsigned int);
__simd128_int16_t __builtin_mve_vqshlq_m_sv8hi(__simd128_int16_t, __simd128_int16_t, __simd128_int16_t, short unsigned int);
__simd128_uint8_t __builtin_mve_vqshlq_m_uv16qi(__simd128_uint8_t, __simd128_uint8_t, __simd128_int8_t, short unsigned int);
__simd128_uint32_t __builtin_mve_vqshlq_m_uv4si(__simd128_uint32_t, __simd128_uint32_t, __simd128_int32_t, short unsigned int);
__simd128_uint16_t __builtin_mve_vqshlq_m_uv8hi(__simd128_uint16_t, __simd128_uint16_t, __simd128_int16_t, short unsigned int);
__simd128_int8_t __builtin_mve_vqshlq_n_sv16qi(__simd128_int8_t, int);
__simd128_int32_t __builtin_mve_vqshlq_n_sv4si(__simd128_int32_t, int);
__simd128_int16_t __builtin_mve_vqshlq_n_sv8hi(__simd128_int16_t, int);
__simd128_uint8_t __builtin_mve_vqshlq_n_uv16qi(__simd128_uint8_t, int);
__simd128_uint32_t __builtin_mve_vqshlq_n_uv4si(__simd128_uint32_t, int);
__simd128_uint16_t __builtin_mve_vqshlq_n_uv8hi(__simd128_uint16_t, int);
__simd128_int8_t __builtin_mve_vqshlq_r_sv16qi(__simd128_int8_t, int);
__simd128_int32_t __builtin_mve_vqshlq_r_sv4si(__simd128_int32_t, int);
__simd128_int16_t __builtin_mve_vqshlq_r_sv8hi(__simd128_int16_t, int);
__simd128_uint8_t __builtin_mve_vqshlq_r_uv16qi(__simd128_uint8_t, int);
__simd128_uint32_t __builtin_mve_vqshlq_r_uv4si(__simd128_uint32_t, int);
__simd128_uint16_t __builtin_mve_vqshlq_r_uv8hi(__simd128_uint16_t, int);
__simd128_int8_t __builtin_mve_vqshlq_sv16qi(__simd128_int8_t, __simd128_int8_t);
__simd128_int32_t __builtin_mve_vqshlq_sv4si(__simd128_int32_t, __simd128_int32_t);
__simd128_int16_t __builtin_mve_vqshlq_sv8hi(__simd128_int16_t, __simd128_int16_t);
__simd128_uint8_t __builtin_mve_vqshlq_uv16qi(__simd128_uint8_t, __simd128_int8_t);
__simd128_uint32_t __builtin_mve_vqshlq_uv4si(__simd128_uint32_t, __simd128_int32_t);
__simd128_uint16_t __builtin_mve_vqshlq_uv8hi(__simd128_uint16_t, __simd128_int16_t);
__simd128_uint8_t __builtin_mve_vqshluq_m_n_sv16qi(__simd128_uint8_t, __simd128_int8_t, int, short unsigned int);
__simd128_uint32_t __builtin_mve_vqshluq_m_n_sv4si(__simd128_uint32_t, __simd128_int32_t, int, short unsigned int);
__simd128_uint16_t __builtin_mve_vqshluq_m_n_sv8hi(__simd128_uint16_t, __simd128_int16_t, int, short unsigned int);
__simd128_uint8_t __builtin_mve_vqshluq_n_sv16qi(__simd128_int8_t, int);
__simd128_uint32_t __builtin_mve_vqshluq_n_sv4si(__simd128_int32_t, int);
__simd128_uint16_t __builtin_mve_vqshluq_n_sv8hi(__simd128_int16_t, int);
__simd128_int16_t __builtin_mve_vqshrnbq_m_n_sv4si(__simd128_int16_t, __simd128_int32_t, int, short unsigned int);
__simd128_int8_t __builtin_mve_vqshrnbq_m_n_sv8hi(__simd128_int8_t, __simd128_int16_t, int, short unsigned int);
__simd128_uint16_t __builtin_mve_vqshrnbq_m_n_uv4si(__simd128_uint16_t, __simd128_uint32_t, int, short unsigned int);
__simd128_uint8_t __builtin_mve_vqshrnbq_m_n_uv8hi(__simd128_uint8_t, __simd128_uint16_t, int, short unsigned int);
__simd128_int16_t __builtin_mve_vqshrnbq_n_sv4si(__simd128_int16_t, __simd128_int32_t, int);
__simd128_int8_t __builtin_mve_vqshrnbq_n_sv8hi(__simd128_int8_t, __simd128_int16_t, int);
__simd128_uint16_t __builtin_mve_vqshrnbq_n_uv4si(__simd128_uint16_t, __simd128_uint32_t, int);
__simd128_uint8_t __builtin_mve_vqshrnbq_n_uv8hi(__simd128_uint8_t, __simd128_uint16_t, int);
__simd128_int16_t __builtin_mve_vqshrntq_m_n_sv4si(__simd128_int16_t, __simd128_int32_t, int, short unsigned int);
__simd128_int8_t __builtin_mve_vqshrntq_m_n_sv8hi(__simd128_int8_t, __simd128_int16_t, int, short unsigned int);
__simd128_uint16_t __builtin_mve_vqshrntq_m_n_uv4si(__simd128_uint16_t, __simd128_uint32_t, int, short unsigned int);
__simd128_uint8_t __builtin_mve_vqshrntq_m_n_uv8hi(__simd128_uint8_t, __simd128_uint16_t, int, short unsigned int);
__simd128_int16_t __builtin_mve_vqshrntq_n_sv4si(__simd128_int16_t, __simd128_int32_t, int);
__simd128_int8_t __builtin_mve_vqshrntq_n_sv8hi(__simd128_int8_t, __simd128_int16_t, int);
__simd128_uint16_t __builtin_mve_vqshrntq_n_uv4si(__simd128_uint16_t, __simd128_uint32_t, int);
__simd128_uint8_t __builtin_mve_vqshrntq_n_uv8hi(__simd128_uint8_t, __simd128_uint16_t, int);
__simd128_uint16_t __builtin_mve_vqshrunbq_m_n_sv4si(__simd128_uint16_t, __simd128_int32_t, int, short unsigned int);
__simd128_uint8_t __builtin_mve_vqshrunbq_m_n_sv8hi(__simd128_uint8_t, __simd128_int16_t, int, short unsigned int);
__simd128_uint16_t __builtin_mve_vqshrunbq_n_sv4si(__simd128_uint16_t, __simd128_int32_t, int);
__simd128_uint8_t __builtin_mve_vqshrunbq_n_sv8hi(__simd128_uint8_t, __simd128_int16_t, int);
__simd128_uint16_t __builtin_mve_vqshruntq_m_n_sv4si(__simd128_uint16_t, __simd128_int32_t, int, short unsigned int);
__simd128_uint8_t __builtin_mve_vqshruntq_m_n_sv8hi(__simd128_uint8_t, __simd128_int16_t, int, short unsigned int);
__simd128_uint16_t __builtin_mve_vqshruntq_n_sv4si(__simd128_uint16_t, __simd128_int32_t, int);
__simd128_uint8_t __builtin_mve_vqshruntq_n_sv8hi(__simd128_uint8_t, __simd128_int16_t, int);
__simd128_int8_t __builtin_mve_vqsubq_m_n_sv16qi(__simd128_int8_t, __simd128_int8_t, signed char, short unsigned int);
__simd128_int32_t __builtin_mve_vqsubq_m_n_sv4si(__simd128_int32_t, __simd128_int32_t, int, short unsigned int);
__simd128_int16_t __builtin_mve_vqsubq_m_n_sv8hi(__simd128_int16_t, __simd128_int16_t, short int, short unsigned int);
__simd128_uint8_t __builtin_mve_vqsubq_m_n_uv16qi(__simd128_uint8_t, __simd128_uint8_t, unsigned char, short unsigned int);
__simd128_uint32_t __builtin_mve_vqsubq_m_n_uv4si(__simd128_uint32_t, __simd128_uint32_t, unsigned int, short unsigned int);
__simd128_uint16_t __builtin_mve_vqsubq_m_n_uv8hi(__simd128_uint16_t, __simd128_uint16_t, short unsigned int, short unsigned int);
__simd128_int8_t __builtin_mve_vqsubq_m_sv16qi(__simd128_int8_t, __simd128_int8_t, __simd128_int8_t, short unsigned int);
__simd128_int32_t __builtin_mve_vqsubq_m_sv4si(__simd128_int32_t, __simd128_int32_t, __simd128_int32_t, short unsigned int);
__simd128_int16_t __builtin_mve_vqsubq_m_sv8hi(__simd128_int16_t, __simd128_int16_t, __simd128_int16_t, short unsigned int);
__simd128_uint8_t __builtin_mve_vqsubq_m_uv16qi(__simd128_uint8_t, __simd128_uint8_t, __simd128_uint8_t, short unsigned int);
__simd128_uint32_t __builtin_mve_vqsubq_m_uv4si(__simd128_uint32_t, __simd128_uint32_t, __simd128_uint32_t, short unsigned int);
__simd128_uint16_t __builtin_mve_vqsubq_m_uv8hi(__simd128_uint16_t, __simd128_uint16_t, __simd128_uint16_t, short unsigned int);
__simd128_int8_t __builtin_mve_vqsubq_n_sv16qi(__simd128_int8_t, signed char);
__simd128_int32_t __builtin_mve_vqsubq_n_sv4si(__simd128_int32_t, int);
__simd128_int16_t __builtin_mve_vqsubq_n_sv8hi(__simd128_int16_t, short int);
__simd128_uint8_t __builtin_mve_vqsubq_n_uv16qi(__simd128_uint8_t, unsigned char);
__simd128_uint32_t __builtin_mve_vqsubq_n_uv4si(__simd128_uint32_t, unsigned int);
__simd128_uint16_t __builtin_mve_vqsubq_n_uv8hi(__simd128_uint16_t, short unsigned int);
__simd128_int8_t __builtin_mve_vqsubq_sv16qi(__simd128_int8_t, __simd128_int8_t);
__simd128_int32_t __builtin_mve_vqsubq_sv4si(__simd128_int32_t, __simd128_int32_t);
__simd128_int16_t __builtin_mve_vqsubq_sv8hi(__simd128_int16_t, __simd128_int16_t);
__simd128_uint8_t __builtin_mve_vqsubq_uv16qi(__simd128_uint8_t, __simd128_uint8_t);
__simd128_uint32_t __builtin_mve_vqsubq_uv4si(__simd128_uint32_t, __simd128_uint32_t);
__simd128_uint16_t __builtin_mve_vqsubq_uv8hi(__simd128_uint16_t, __simd128_uint16_t);
__simd128_int8_t __builtin_mve_vrev16q_m_sv16qi(__simd128_int8_t, __simd128_int8_t, short unsigned int);
__simd128_uint8_t __builtin_mve_vrev16q_m_uv16qi(__simd128_uint8_t, __simd128_uint8_t, short unsigned int);
__simd128_int8_t __builtin_mve_vrev16q_sv16qi(__simd128_int8_t);
__simd128_uint8_t __builtin_mve_vrev16q_uv16qi(__simd128_uint8_t);
__simd128_float16_t __builtin_mve_vrev32q_fv8hf(__simd128_float16_t);
__simd128_float16_t __builtin_mve_vrev32q_m_fv8hf(__simd128_float16_t, __simd128_float16_t, short unsigned int);
__simd128_int8_t __builtin_mve_vrev32q_m_sv16qi(__simd128_int8_t, __simd128_int8_t, short unsigned int);
__simd128_int16_t __builtin_mve_vrev32q_m_sv8hi(__simd128_int16_t, __simd128_int16_t, short unsigned int);
__simd128_uint8_t __builtin_mve_vrev32q_m_uv16qi(__simd128_uint8_t, __simd128_uint8_t, short unsigned int);
__simd128_uint16_t __builtin_mve_vrev32q_m_uv8hi(__simd128_uint16_t, __simd128_uint16_t, short unsigned int);
__simd128_int8_t __builtin_mve_vrev32q_sv16qi(__simd128_int8_t);
__simd128_int16_t __builtin_mve_vrev32q_sv8hi(__simd128_int16_t);
__simd128_uint8_t __builtin_mve_vrev32q_uv16qi(__simd128_uint8_t);
__simd128_uint16_t __builtin_mve_vrev32q_uv8hi(__simd128_uint16_t);
__simd128_float32_t __builtin_mve_vrev64q_fv4sf(__simd128_float32_t);
__simd128_float16_t __builtin_mve_vrev64q_fv8hf(__simd128_float16_t);
__simd128_float32_t __builtin_mve_vrev64q_m_fv4sf(__simd128_float32_t, __simd128_float32_t, short unsigned int);
__simd128_float16_t __builtin_mve_vrev64q_m_fv8hf(__simd128_float16_t, __simd128_float16_t, short unsigned int);
__simd128_int8_t __builtin_mve_vrev64q_m_sv16qi(__simd128_int8_t, __simd128_int8_t, short unsigned int);
__simd128_int32_t __builtin_mve_vrev64q_m_sv4si(__simd128_int32_t, __simd128_int32_t, short unsigned int);
__simd128_int16_t __builtin_mve_vrev64q_m_sv8hi(__simd128_int16_t, __simd128_int16_t, short unsigned int);
__simd128_uint8_t __builtin_mve_vrev64q_m_uv16qi(__simd128_uint8_t, __simd128_uint8_t, short unsigned int);
__simd128_uint32_t __builtin_mve_vrev64q_m_uv4si(__simd128_uint32_t, __simd128_uint32_t, short unsigned int);
__simd128_uint16_t __builtin_mve_vrev64q_m_uv8hi(__simd128_uint16_t, __simd128_uint16_t, short unsigned int);
__simd128_int8_t __builtin_mve_vrev64q_sv16qi(__simd128_int8_t);
__simd128_int32_t __builtin_mve_vrev64q_sv4si(__simd128_int32_t);
__simd128_int16_t __builtin_mve_vrev64q_sv8hi(__simd128_int16_t);
__simd128_uint8_t __builtin_mve_vrev64q_uv16qi(__simd128_uint8_t);
__simd128_uint32_t __builtin_mve_vrev64q_uv4si(__simd128_uint32_t);
__simd128_uint16_t __builtin_mve_vrev64q_uv8hi(__simd128_uint16_t);
__simd128_int8_t __builtin_mve_vrhaddq_m_sv16qi(__simd128_int8_t, __simd128_int8_t, __simd128_int8_t, short unsigned int);
__simd128_int32_t __builtin_mve_vrhaddq_m_sv4si(__simd128_int32_t, __simd128_int32_t, __simd128_int32_t, short unsigned int);
__simd128_int16_t __builtin_mve_vrhaddq_m_sv8hi(__simd128_int16_t, __simd128_int16_t, __simd128_int16_t, short unsigned int);
__simd128_uint8_t __builtin_mve_vrhaddq_m_uv16qi(__simd128_uint8_t, __simd128_uint8_t, __simd128_uint8_t, short unsigned int);
__simd128_uint32_t __builtin_mve_vrhaddq_m_uv4si(__simd128_uint32_t, __simd128_uint32_t, __simd128_uint32_t, short unsigned int);
__simd128_uint16_t __builtin_mve_vrhaddq_m_uv8hi(__simd128_uint16_t, __simd128_uint16_t, __simd128_uint16_t, short unsigned int);
__simd128_int8_t __builtin_mve_vrhaddq_sv16qi(__simd128_int8_t, __simd128_int8_t);
__simd128_int32_t __builtin_mve_vrhaddq_sv4si(__simd128_int32_t, __simd128_int32_t);
__simd128_int16_t __builtin_mve_vrhaddq_sv8hi(__simd128_int16_t, __simd128_int16_t);
__simd128_uint8_t __builtin_mve_vrhaddq_uv16qi(__simd128_uint8_t, __simd128_uint8_t);
__simd128_uint32_t __builtin_mve_vrhaddq_uv4si(__simd128_uint32_t, __simd128_uint32_t);
__simd128_uint16_t __builtin_mve_vrhaddq_uv8hi(__simd128_uint16_t, __simd128_uint16_t);
long long int __builtin_mve_vrmlaldavhaq_p_sv4si(long long int, __simd128_int32_t, __simd128_int32_t, short unsigned int);
long long unsigned int __builtin_mve_vrmlaldavhaq_p_uv4si(long long unsigned int, __simd128_uint32_t, __simd128_uint32_t, short unsigned int);
long long int __builtin_mve_vrmlaldavhaq_sv4si(long long int, __simd128_int32_t, __simd128_int32_t);
long long unsigned int __builtin_mve_vrmlaldavhaq_uv4si(long long unsigned int, __simd128_uint32_t, __simd128_uint32_t);
long long int __builtin_mve_vrmlaldavhaxq_p_sv4si(long long int, __simd128_int32_t, __simd128_int32_t, short unsigned int);
long long int __builtin_mve_vrmlaldavhaxq_sv4si(long long int, __simd128_int32_t, __simd128_int32_t);
long long int __builtin_mve_vrmlaldavhq_p_sv4si(__simd128_int32_t, __simd128_int32_t, short unsigned int);
long long unsigned int __builtin_mve_vrmlaldavhq_p_uv4si(__simd128_uint32_t, __simd128_uint32_t, short unsigned int);
long long int __builtin_mve_vrmlaldavhq_sv4si(__simd128_int32_t, __simd128_int32_t);
long long unsigned int __builtin_mve_vrmlaldavhq_uv4si(__simd128_uint32_t, __simd128_uint32_t);
long long int __builtin_mve_vrmlaldavhxq_p_sv4si(__simd128_int32_t, __simd128_int32_t, short unsigned int);
long long int __builtin_mve_vrmlaldavhxq_sv4si(__simd128_int32_t, __simd128_int32_t);
long long int __builtin_mve_vrmlsldavhaq_p_sv4si(long long int, __simd128_int32_t, __simd128_int32_t, short unsigned int);
long long int __builtin_mve_vrmlsldavhaq_sv4si(long long int, __simd128_int32_t, __simd128_int32_t);
long long int __builtin_mve_vrmlsldavhaxq_p_sv4si(long long int, __simd128_int32_t, __simd128_int32_t, short unsigned int);
long long int __builtin_mve_vrmlsldavhaxq_sv4si(long long int, __simd128_int32_t, __simd128_int32_t);
long long int __builtin_mve_vrmlsldavhq_p_sv4si(__simd128_int32_t, __simd128_int32_t, short unsigned int);
long long int __builtin_mve_vrmlsldavhq_sv4si(__simd128_int32_t, __simd128_int32_t);
long long int __builtin_mve_vrmlsldavhxq_p_sv4si(__simd128_int32_t, __simd128_int32_t, short unsigned int);
long long int __builtin_mve_vrmlsldavhxq_sv4si(__simd128_int32_t, __simd128_int32_t);
__simd128_int8_t __builtin_mve_vrmulhq_m_sv16qi(__simd128_int8_t, __simd128_int8_t, __simd128_int8_t, short unsigned int);
__simd128_int32_t __builtin_mve_vrmulhq_m_sv4si(__simd128_int32_t, __simd128_int32_t, __simd128_int32_t, short unsigned int);
__simd128_int16_t __builtin_mve_vrmulhq_m_sv8hi(__simd128_int16_t, __simd128_int16_t, __simd128_int16_t, short unsigned int);
__simd128_uint8_t __builtin_mve_vrmulhq_m_uv16qi(__simd128_uint8_t, __simd128_uint8_t, __simd128_uint8_t, short unsigned int);
__simd128_uint32_t __builtin_mve_vrmulhq_m_uv4si(__simd128_uint32_t, __simd128_uint32_t, __simd128_uint32_t, short unsigned int);
__simd128_uint16_t __builtin_mve_vrmulhq_m_uv8hi(__simd128_uint16_t, __simd128_uint16_t, __simd128_uint16_t, short unsigned int);
__simd128_int8_t __builtin_mve_vrmulhq_sv16qi(__simd128_int8_t, __simd128_int8_t);
__simd128_int32_t __builtin_mve_vrmulhq_sv4si(__simd128_int32_t, __simd128_int32_t);
__simd128_int16_t __builtin_mve_vrmulhq_sv8hi(__simd128_int16_t, __simd128_int16_t);
__simd128_uint8_t __builtin_mve_vrmulhq_uv16qi(__simd128_uint8_t, __simd128_uint8_t);
__simd128_uint32_t __builtin_mve_vrmulhq_uv4si(__simd128_uint32_t, __simd128_uint32_t);
__simd128_uint16_t __builtin_mve_vrmulhq_uv8hi(__simd128_uint16_t, __simd128_uint16_t);
__simd128_float32_t __builtin_mve_vrndaq_fv4sf(__simd128_float32_t);
__simd128_float16_t __builtin_mve_vrndaq_fv8hf(__simd128_float16_t);
__simd128_float32_t __builtin_mve_vrndaq_m_fv4sf(__simd128_float32_t, __simd128_float32_t, short unsigned int);
__simd128_float16_t __builtin_mve_vrndaq_m_fv8hf(__simd128_float16_t, __simd128_float16_t, short unsigned int);
__simd128_float32_t __builtin_mve_vrndmq_fv4sf(__simd128_float32_t);
__simd128_float16_t __builtin_mve_vrndmq_fv8hf(__simd128_float16_t);
__simd128_float32_t __builtin_mve_vrndmq_m_fv4sf(__simd128_float32_t, __simd128_float32_t, short unsigned int);
__simd128_float16_t __builtin_mve_vrndmq_m_fv8hf(__simd128_float16_t, __simd128_float16_t, short unsigned int);
__simd128_float32_t __builtin_mve_vrndnq_fv4sf(__simd128_float32_t);
__simd128_float16_t __builtin_mve_vrndnq_fv8hf(__simd128_float16_t);
__simd128_float32_t __builtin_mve_vrndnq_m_fv4sf(__simd128_float32_t, __simd128_float32_t, short unsigned int);
__simd128_float16_t __builtin_mve_vrndnq_m_fv8hf(__simd128_float16_t, __simd128_float16_t, short unsigned int);
__simd128_float32_t __builtin_mve_vrndpq_fv4sf(__simd128_float32_t);
__simd128_float16_t __builtin_mve_vrndpq_fv8hf(__simd128_float16_t);
__simd128_float32_t __builtin_mve_vrndpq_m_fv4sf(__simd128_float32_t, __simd128_float32_t, short unsigned int);
__simd128_float16_t __builtin_mve_vrndpq_m_fv8hf(__simd128_float16_t, __simd128_float16_t, short unsigned int);
__simd128_float32_t __builtin_mve_vrndq_fv4sf(__simd128_float32_t);
__simd128_float16_t __builtin_mve_vrndq_fv8hf(__simd128_float16_t);
__simd128_float32_t __builtin_mve_vrndq_m_fv4sf(__simd128_float32_t, __simd128_float32_t, short unsigned int);
__simd128_float16_t __builtin_mve_vrndq_m_fv8hf(__simd128_float16_t, __simd128_float16_t, short unsigned int);
__simd128_float32_t __builtin_mve_vrndxq_fv4sf(__simd128_float32_t);
__simd128_float16_t __builtin_mve_vrndxq_fv8hf(__simd128_float16_t);
__simd128_float32_t __builtin_mve_vrndxq_m_fv4sf(__simd128_float32_t, __simd128_float32_t, short unsigned int);
__simd128_float16_t __builtin_mve_vrndxq_m_fv8hf(__simd128_float16_t, __simd128_float16_t, short unsigned int);
__simd128_int8_t __builtin_mve_vrshlq_m_n_sv16qi(__simd128_int8_t, int, short unsigned int);
__simd128_int32_t __builtin_mve_vrshlq_m_n_sv4si(__simd128_int32_t, int, short unsigned int);
__simd128_int16_t __builtin_mve_vrshlq_m_n_sv8hi(__simd128_int16_t, int, short unsigned int);
__simd128_uint8_t __builtin_mve_vrshlq_m_n_uv16qi(__simd128_uint8_t, int, short unsigned int);
__simd128_uint32_t __builtin_mve_vrshlq_m_n_uv4si(__simd128_uint32_t, int, short unsigned int);
__simd128_uint16_t __builtin_mve_vrshlq_m_n_uv8hi(__simd128_uint16_t, int, short unsigned int);
__simd128_int8_t __builtin_mve_vrshlq_m_sv16qi(__simd128_int8_t, __simd128_int8_t, __simd128_int8_t, short unsigned int);
__simd128_int32_t __builtin_mve_vrshlq_m_sv4si(__simd128_int32_t, __simd128_int32_t, __simd128_int32_t, short unsigned int);
__simd128_int16_t __builtin_mve_vrshlq_m_sv8hi(__simd128_int16_t, __simd128_int16_t, __simd128_int16_t, short unsigned int);
__simd128_uint8_t __builtin_mve_vrshlq_m_uv16qi(__simd128_uint8_t, __simd128_uint8_t, __simd128_int8_t, short unsigned int);
__simd128_uint32_t __builtin_mve_vrshlq_m_uv4si(__simd128_uint32_t, __simd128_uint32_t, __simd128_int32_t, short unsigned int);
__simd128_uint16_t __builtin_mve_vrshlq_m_uv8hi(__simd128_uint16_t, __simd128_uint16_t, __simd128_int16_t, short unsigned int);
__simd128_int8_t __builtin_mve_vrshlq_n_sv16qi(__simd128_int8_t, int);
__simd128_int32_t __builtin_mve_vrshlq_n_sv4si(__simd128_int32_t, int);
__simd128_int16_t __builtin_mve_vrshlq_n_sv8hi(__simd128_int16_t, int);
__simd128_uint8_t __builtin_mve_vrshlq_n_uv16qi(__simd128_uint8_t, int);
__simd128_uint32_t __builtin_mve_vrshlq_n_uv4si(__simd128_uint32_t, int);
__simd128_uint16_t __builtin_mve_vrshlq_n_uv8hi(__simd128_uint16_t, int);
__simd128_int8_t __builtin_mve_vrshlq_sv16qi(__simd128_int8_t, __simd128_int8_t);
__simd128_int32_t __builtin_mve_vrshlq_sv4si(__simd128_int32_t, __simd128_int32_t);
__simd128_int16_t __builtin_mve_vrshlq_sv8hi(__simd128_int16_t, __simd128_int16_t);
__simd128_uint8_t __builtin_mve_vrshlq_uv16qi(__simd128_uint8_t, __simd128_int8_t);
__simd128_uint32_t __builtin_mve_vrshlq_uv4si(__simd128_uint32_t, __simd128_int32_t);
__simd128_uint16_t __builtin_mve_vrshlq_uv8hi(__simd128_uint16_t, __simd128_int16_t);
__simd128_int16_t __builtin_mve_vrshrnbq_m_n_sv4si(__simd128_int16_t, __simd128_int32_t, int, short unsigned int);
__simd128_int8_t __builtin_mve_vrshrnbq_m_n_sv8hi(__simd128_int8_t, __simd128_int16_t, int, short unsigned int);
__simd128_uint16_t __builtin_mve_vrshrnbq_m_n_uv4si(__simd128_uint16_t, __simd128_uint32_t, int, short unsigned int);
__simd128_uint8_t __builtin_mve_vrshrnbq_m_n_uv8hi(__simd128_uint8_t, __simd128_uint16_t, int, short unsigned int);
__simd128_int16_t __builtin_mve_vrshrnbq_n_sv4si(__simd128_int16_t, __simd128_int32_t, int);
__simd128_int8_t __builtin_mve_vrshrnbq_n_sv8hi(__simd128_int8_t, __simd128_int16_t, int);
__simd128_uint16_t __builtin_mve_vrshrnbq_n_uv4si(__simd128_uint16_t, __simd128_uint32_t, int);
__simd128_uint8_t __builtin_mve_vrshrnbq_n_uv8hi(__simd128_uint8_t, __simd128_uint16_t, int);
__simd128_int16_t __builtin_mve_vrshrntq_m_n_sv4si(__simd128_int16_t, __simd128_int32_t, int, short unsigned int);
__simd128_int8_t __builtin_mve_vrshrntq_m_n_sv8hi(__simd128_int8_t, __simd128_int16_t, int, short unsigned int);
__simd128_uint16_t __builtin_mve_vrshrntq_m_n_uv4si(__simd128_uint16_t, __simd128_uint32_t, int, short unsigned int);
__simd128_uint8_t __builtin_mve_vrshrntq_m_n_uv8hi(__simd128_uint8_t, __simd128_uint16_t, int, short unsigned int);
__simd128_int16_t __builtin_mve_vrshrntq_n_sv4si(__simd128_int16_t, __simd128_int32_t, int);
__simd128_int8_t __builtin_mve_vrshrntq_n_sv8hi(__simd128_int8_t, __simd128_int16_t, int);
__simd128_uint16_t __builtin_mve_vrshrntq_n_uv4si(__simd128_uint16_t, __simd128_uint32_t, int);
__simd128_uint8_t __builtin_mve_vrshrntq_n_uv8hi(__simd128_uint8_t, __simd128_uint16_t, int);
__simd128_int8_t __builtin_mve_vrshrq_m_n_sv16qi(__simd128_int8_t, __simd128_int8_t, int, short unsigned int);
__simd128_int32_t __builtin_mve_vrshrq_m_n_sv4si(__simd128_int32_t, __simd128_int32_t, int, short unsigned int);
__simd128_int16_t __builtin_mve_vrshrq_m_n_sv8hi(__simd128_int16_t, __simd128_int16_t, int, short unsigned int);
__simd128_uint8_t __builtin_mve_vrshrq_m_n_uv16qi(__simd128_uint8_t, __simd128_uint8_t, int, short unsigned int);
__simd128_uint32_t __builtin_mve_vrshrq_m_n_uv4si(__simd128_uint32_t, __simd128_uint32_t, int, short unsigned int);
__simd128_uint16_t __builtin_mve_vrshrq_m_n_uv8hi(__simd128_uint16_t, __simd128_uint16_t, int, short unsigned int);
__simd128_int8_t __builtin_mve_vrshrq_n_sv16qi(__simd128_int8_t, int);
__simd128_int32_t __builtin_mve_vrshrq_n_sv4si(__simd128_int32_t, int);
__simd128_int16_t __builtin_mve_vrshrq_n_sv8hi(__simd128_int16_t, int);
__simd128_uint8_t __builtin_mve_vrshrq_n_uv16qi(__simd128_uint8_t, int);
__simd128_uint32_t __builtin_mve_vrshrq_n_uv4si(__simd128_uint32_t, int);
__simd128_uint16_t __builtin_mve_vrshrq_n_uv8hi(__simd128_uint16_t, int);
__simd128_int32_t __builtin_mve_vsbciq_m_sv4si(__simd128_int32_t, __simd128_int32_t, __simd128_int32_t, short unsigned int);
__simd128_uint32_t __builtin_mve_vsbciq_m_uv4si(__simd128_uint32_t, __simd128_uint32_t, __simd128_uint32_t, short unsigned int);
__simd128_int32_t __builtin_mve_vsbciq_sv4si(__simd128_int32_t, __simd128_int32_t);
__simd128_uint32_t __builtin_mve_vsbciq_uv4si(__simd128_uint32_t, __simd128_uint32_t);
__simd128_int32_t __builtin_mve_vsbcq_m_sv4si(__simd128_int32_t, __simd128_int32_t, __simd128_int32_t, short unsigned int);
__simd128_uint32_t __builtin_mve_vsbcq_m_uv4si(__simd128_uint32_t, __simd128_uint32_t, __simd128_uint32_t, short unsigned int);
__simd128_int32_t __builtin_mve_vsbcq_sv4si(__simd128_int32_t, __simd128_int32_t);
__simd128_uint32_t __builtin_mve_vsbcq_uv4si(__simd128_uint32_t, __simd128_uint32_t);
unsigned int __builtin_mve_vshlcq_carry_sv16qi(__simd128_int8_t, unsigned int, int);
unsigned int __builtin_mve_vshlcq_carry_sv4si(__simd128_int32_t, unsigned int, int);
unsigned int __builtin_mve_vshlcq_carry_sv8hi(__simd128_int16_t, unsigned int, int);
unsigned int __builtin_mve_vshlcq_carry_uv16qi(__simd128_uint8_t, unsigned int, int);
unsigned int __builtin_mve_vshlcq_carry_uv4si(__simd128_uint32_t, unsigned int, int);
unsigned int __builtin_mve_vshlcq_carry_uv8hi(__simd128_uint16_t, unsigned int, int);
int __builtin_mve_vshlcq_m_carry_sv16qi(__simd128_int8_t, unsigned int, int, short unsigned int);
int __builtin_mve_vshlcq_m_carry_sv4si(__simd128_int32_t, unsigned int, int, short unsigned int);
int __builtin_mve_vshlcq_m_carry_sv8hi(__simd128_int16_t, unsigned int, int, short unsigned int);
unsigned int __builtin_mve_vshlcq_m_carry_uv16qi(__simd128_uint8_t, unsigned int, int, short unsigned int);
unsigned int __builtin_mve_vshlcq_m_carry_uv4si(__simd128_uint32_t, unsigned int, int, short unsigned int);
unsigned int __builtin_mve_vshlcq_m_carry_uv8hi(__simd128_uint16_t, unsigned int, int, short unsigned int);
__simd128_int8_t __builtin_mve_vshlcq_m_vec_sv16qi(__simd128_int8_t, unsigned int, int, short unsigned int);
__simd128_int32_t __builtin_mve_vshlcq_m_vec_sv4si(__simd128_int32_t, unsigned int, int, short unsigned int);
__simd128_int16_t __builtin_mve_vshlcq_m_vec_sv8hi(__simd128_int16_t, unsigned int, int, short unsigned int);
__simd128_uint8_t __builtin_mve_vshlcq_m_vec_uv16qi(__simd128_uint8_t, unsigned int, int, short unsigned int);
__simd128_uint32_t __builtin_mve_vshlcq_m_vec_uv4si(__simd128_uint32_t, unsigned int, int, short unsigned int);
__simd128_uint16_t __builtin_mve_vshlcq_m_vec_uv8hi(__simd128_uint16_t, unsigned int, int, short unsigned int);
__simd128_int8_t __builtin_mve_vshlcq_vec_sv16qi(__simd128_int8_t, unsigned int, int);
__simd128_int32_t __builtin_mve_vshlcq_vec_sv4si(__simd128_int32_t, unsigned int, int);
__simd128_int16_t __builtin_mve_vshlcq_vec_sv8hi(__simd128_int16_t, unsigned int, int);
__simd128_uint8_t __builtin_mve_vshlcq_vec_uv16qi(__simd128_uint8_t, unsigned int, int);
__simd128_uint32_t __builtin_mve_vshlcq_vec_uv4si(__simd128_uint32_t, unsigned int, int);
__simd128_uint16_t __builtin_mve_vshlcq_vec_uv8hi(__simd128_uint16_t, unsigned int, int);
__simd128_int16_t __builtin_mve_vshllbq_m_n_sv16qi(__simd128_int16_t, __simd128_int8_t, int, short unsigned int);
__simd128_int32_t __builtin_mve_vshllbq_m_n_sv8hi(__simd128_int32_t, __simd128_int16_t, int, short unsigned int);
__simd128_uint16_t __builtin_mve_vshllbq_m_n_uv16qi(__simd128_uint16_t, __simd128_uint8_t, int, short unsigned int);
__simd128_uint32_t __builtin_mve_vshllbq_m_n_uv8hi(__simd128_uint32_t, __simd128_uint16_t, int, short unsigned int);
__simd128_int16_t __builtin_mve_vshllbq_n_sv16qi(__simd128_int8_t, int);
__simd128_int32_t __builtin_mve_vshllbq_n_sv8hi(__simd128_int16_t, int);
__simd128_uint16_t __builtin_mve_vshllbq_n_uv16qi(__simd128_uint8_t, int);
__simd128_uint32_t __builtin_mve_vshllbq_n_uv8hi(__simd128_uint16_t, int);
__simd128_int16_t __builtin_mve_vshlltq_m_n_sv16qi(__simd128_int16_t, __simd128_int8_t, int, short unsigned int);
__simd128_int32_t __builtin_mve_vshlltq_m_n_sv8hi(__simd128_int32_t, __simd128_int16_t, int, short unsigned int);
__simd128_uint16_t __builtin_mve_vshlltq_m_n_uv16qi(__simd128_uint16_t, __simd128_uint8_t, int, short unsigned int);
__simd128_uint32_t __builtin_mve_vshlltq_m_n_uv8hi(__simd128_uint32_t, __simd128_uint16_t, int, short unsigned int);
__simd128_int16_t __builtin_mve_vshlltq_n_sv16qi(__simd128_int8_t, int);
__simd128_int32_t __builtin_mve_vshlltq_n_sv8hi(__simd128_int16_t, int);
__simd128_uint16_t __builtin_mve_vshlltq_n_uv16qi(__simd128_uint8_t, int);
__simd128_uint32_t __builtin_mve_vshlltq_n_uv8hi(__simd128_uint16_t, int);
__simd128_int8_t __builtin_mve_vshlq_m_n_sv16qi(__simd128_int8_t, __simd128_int8_t, int, short unsigned int);
__simd128_int32_t __builtin_mve_vshlq_m_n_sv4si(__simd128_int32_t, __simd128_int32_t, int, short unsigned int);
__simd128_int16_t __builtin_mve_vshlq_m_n_sv8hi(__simd128_int16_t, __simd128_int16_t, int, short unsigned int);
__simd128_uint8_t __builtin_mve_vshlq_m_n_uv16qi(__simd128_uint8_t, __simd128_uint8_t, int, short unsigned int);
__simd128_uint32_t __builtin_mve_vshlq_m_n_uv4si(__simd128_uint32_t, __simd128_uint32_t, int, short unsigned int);
__simd128_uint16_t __builtin_mve_vshlq_m_n_uv8hi(__simd128_uint16_t, __simd128_uint16_t, int, short unsigned int);
__simd128_int8_t __builtin_mve_vshlq_m_r_sv16qi(__simd128_int8_t, int, short unsigned int);
__simd128_int32_t __builtin_mve_vshlq_m_r_sv4si(__simd128_int32_t, int, short unsigned int);
__simd128_int16_t __builtin_mve_vshlq_m_r_sv8hi(__simd128_int16_t, int, short unsigned int);
__simd128_uint8_t __builtin_mve_vshlq_m_r_uv16qi(__simd128_uint8_t, int, short unsigned int);
__simd128_uint32_t __builtin_mve_vshlq_m_r_uv4si(__simd128_uint32_t, int, short unsigned int);
__simd128_uint16_t __builtin_mve_vshlq_m_r_uv8hi(__simd128_uint16_t, int, short unsigned int);
__simd128_int8_t __builtin_mve_vshlq_m_sv16qi(__simd128_int8_t, __simd128_int8_t, __simd128_int8_t, short unsigned int);
__simd128_int32_t __builtin_mve_vshlq_m_sv4si(__simd128_int32_t, __simd128_int32_t, __simd128_int32_t, short unsigned int);
__simd128_int16_t __builtin_mve_vshlq_m_sv8hi(__simd128_int16_t, __simd128_int16_t, __simd128_int16_t, short unsigned int);
__simd128_uint8_t __builtin_mve_vshlq_m_uv16qi(__simd128_uint8_t, __simd128_uint8_t, __simd128_int8_t, short unsigned int);
__simd128_uint32_t __builtin_mve_vshlq_m_uv4si(__simd128_uint32_t, __simd128_uint32_t, __simd128_int32_t, short unsigned int);
__simd128_uint16_t __builtin_mve_vshlq_m_uv8hi(__simd128_uint16_t, __simd128_uint16_t, __simd128_int16_t, short unsigned int);
__simd128_int8_t __builtin_mve_vshlq_n_sv16qi(__simd128_int8_t, int);
__simd128_int32_t __builtin_mve_vshlq_n_sv4si(__simd128_int32_t, int);
__simd128_int16_t __builtin_mve_vshlq_n_sv8hi(__simd128_int16_t, int);
__simd128_uint8_t __builtin_mve_vshlq_n_uv16qi(__simd128_uint8_t, int);
__simd128_uint32_t __builtin_mve_vshlq_n_uv4si(__simd128_uint32_t, int);
__simd128_uint16_t __builtin_mve_vshlq_n_uv8hi(__simd128_uint16_t, int);
__simd128_int8_t __builtin_mve_vshlq_r_sv16qi(__simd128_int8_t, int);
__simd128_int32_t __builtin_mve_vshlq_r_sv4si(__simd128_int32_t, int);
__simd128_int16_t __builtin_mve_vshlq_r_sv8hi(__simd128_int16_t, int);
__simd128_uint8_t __builtin_mve_vshlq_r_uv16qi(__simd128_uint8_t, int);
__simd128_uint32_t __builtin_mve_vshlq_r_uv4si(__simd128_uint32_t, int);
__simd128_uint16_t __builtin_mve_vshlq_r_uv8hi(__simd128_uint16_t, int);
__simd128_int8_t __builtin_mve_vshlq_sv16qi(__simd128_int8_t, __simd128_int8_t);
__simd128_int32_t __builtin_mve_vshlq_sv4si(__simd128_int32_t, __simd128_int32_t);
__simd128_int16_t __builtin_mve_vshlq_sv8hi(__simd128_int16_t, __simd128_int16_t);
__simd128_uint8_t __builtin_mve_vshlq_uv16qi(__simd128_uint8_t, __simd128_int8_t);
__simd128_uint32_t __builtin_mve_vshlq_uv4si(__simd128_uint32_t, __simd128_int32_t);
__simd128_uint16_t __builtin_mve_vshlq_uv8hi(__simd128_uint16_t, __simd128_int16_t);
__simd128_int16_t __builtin_mve_vshrnbq_m_n_sv4si(__simd128_int16_t, __simd128_int32_t, int, short unsigned int);
__simd128_int8_t __builtin_mve_vshrnbq_m_n_sv8hi(__simd128_int8_t, __simd128_int16_t, int, short unsigned int);
__simd128_uint16_t __builtin_mve_vshrnbq_m_n_uv4si(__simd128_uint16_t, __simd128_uint32_t, int, short unsigned int);
__simd128_uint8_t __builtin_mve_vshrnbq_m_n_uv8hi(__simd128_uint8_t, __simd128_uint16_t, int, short unsigned int);
__simd128_int16_t __builtin_mve_vshrnbq_n_sv4si(__simd128_int16_t, __simd128_int32_t, int);
__simd128_int8_t __builtin_mve_vshrnbq_n_sv8hi(__simd128_int8_t, __simd128_int16_t, int);
__simd128_uint16_t __builtin_mve_vshrnbq_n_uv4si(__simd128_uint16_t, __simd128_uint32_t, int);
__simd128_uint8_t __builtin_mve_vshrnbq_n_uv8hi(__simd128_uint8_t, __simd128_uint16_t, int);
__simd128_int16_t __builtin_mve_vshrntq_m_n_sv4si(__simd128_int16_t, __simd128_int32_t, int, short unsigned int);
__simd128_int8_t __builtin_mve_vshrntq_m_n_sv8hi(__simd128_int8_t, __simd128_int16_t, int, short unsigned int);
__simd128_uint16_t __builtin_mve_vshrntq_m_n_uv4si(__simd128_uint16_t, __simd128_uint32_t, int, short unsigned int);
__simd128_uint8_t __builtin_mve_vshrntq_m_n_uv8hi(__simd128_uint8_t, __simd128_uint16_t, int, short unsigned int);
__simd128_int16_t __builtin_mve_vshrntq_n_sv4si(__simd128_int16_t, __simd128_int32_t, int);
__simd128_int8_t __builtin_mve_vshrntq_n_sv8hi(__simd128_int8_t, __simd128_int16_t, int);
__simd128_uint16_t __builtin_mve_vshrntq_n_uv4si(__simd128_uint16_t, __simd128_uint32_t, int);
__simd128_uint8_t __builtin_mve_vshrntq_n_uv8hi(__simd128_uint8_t, __simd128_uint16_t, int);
__simd128_int8_t __builtin_mve_vshrq_m_n_sv16qi(__simd128_int8_t, __simd128_int8_t, int, short unsigned int);
__simd128_int32_t __builtin_mve_vshrq_m_n_sv4si(__simd128_int32_t, __simd128_int32_t, int, short unsigned int);
__simd128_int16_t __builtin_mve_vshrq_m_n_sv8hi(__simd128_int16_t, __simd128_int16_t, int, short unsigned int);
__simd128_uint8_t __builtin_mve_vshrq_m_n_uv16qi(__simd128_uint8_t, __simd128_uint8_t, int, short unsigned int);
__simd128_uint32_t __builtin_mve_vshrq_m_n_uv4si(__simd128_uint32_t, __simd128_uint32_t, int, short unsigned int);
__simd128_uint16_t __builtin_mve_vshrq_m_n_uv8hi(__simd128_uint16_t, __simd128_uint16_t, int, short unsigned int);
__simd128_int8_t __builtin_mve_vshrq_n_sv16qi(__simd128_int8_t, int);
__simd128_int32_t __builtin_mve_vshrq_n_sv4si(__simd128_int32_t, int);
__simd128_int16_t __builtin_mve_vshrq_n_sv8hi(__simd128_int16_t, int);
__simd128_uint8_t __builtin_mve_vshrq_n_uv16qi(__simd128_uint8_t, int);
__simd128_uint32_t __builtin_mve_vshrq_n_uv4si(__simd128_uint32_t, int);
__simd128_uint16_t __builtin_mve_vshrq_n_uv8hi(__simd128_uint16_t, int);
__simd128_int8_t __builtin_mve_vsliq_m_n_sv16qi(__simd128_int8_t, __simd128_int8_t, int, short unsigned int);
__simd128_int32_t __builtin_mve_vsliq_m_n_sv4si(__simd128_int32_t, __simd128_int32_t, int, short unsigned int);
__simd128_int16_t __builtin_mve_vsliq_m_n_sv8hi(__simd128_int16_t, __simd128_int16_t, int, short unsigned int);
__simd128_uint8_t __builtin_mve_vsliq_m_n_uv16qi(__simd128_uint8_t, __simd128_uint8_t, int, short unsigned int);
__simd128_uint32_t __builtin_mve_vsliq_m_n_uv4si(__simd128_uint32_t, __simd128_uint32_t, int, short unsigned int);
__simd128_uint16_t __builtin_mve_vsliq_m_n_uv8hi(__simd128_uint16_t, __simd128_uint16_t, int, short unsigned int);
__simd128_int8_t __builtin_mve_vsliq_n_sv16qi(__simd128_int8_t, __simd128_int8_t, int);
__simd128_int32_t __builtin_mve_vsliq_n_sv4si(__simd128_int32_t, __simd128_int32_t, int);
__simd128_int16_t __builtin_mve_vsliq_n_sv8hi(__simd128_int16_t, __simd128_int16_t, int);
__simd128_uint8_t __builtin_mve_vsliq_n_uv16qi(__simd128_uint8_t, __simd128_uint8_t, int);
__simd128_uint32_t __builtin_mve_vsliq_n_uv4si(__simd128_uint32_t, __simd128_uint32_t, int);
__simd128_uint16_t __builtin_mve_vsliq_n_uv8hi(__simd128_uint16_t, __simd128_uint16_t, int);
__simd128_int8_t __builtin_mve_vsriq_m_n_sv16qi(__simd128_int8_t, __simd128_int8_t, int, short unsigned int);
__simd128_int32_t __builtin_mve_vsriq_m_n_sv4si(__simd128_int32_t, __simd128_int32_t, int, short unsigned int);
__simd128_int16_t __builtin_mve_vsriq_m_n_sv8hi(__simd128_int16_t, __simd128_int16_t, int, short unsigned int);
__simd128_uint8_t __builtin_mve_vsriq_m_n_uv16qi(__simd128_uint8_t, __simd128_uint8_t, int, short unsigned int);
__simd128_uint32_t __builtin_mve_vsriq_m_n_uv4si(__simd128_uint32_t, __simd128_uint32_t, int, short unsigned int);
__simd128_uint16_t __builtin_mve_vsriq_m_n_uv8hi(__simd128_uint16_t, __simd128_uint16_t, int, short unsigned int);
__simd128_int8_t __builtin_mve_vsriq_n_sv16qi(__simd128_int8_t, __simd128_int8_t, int);
__simd128_int32_t __builtin_mve_vsriq_n_sv4si(__simd128_int32_t, __simd128_int32_t, int);
__simd128_int16_t __builtin_mve_vsriq_n_sv8hi(__simd128_int16_t, __simd128_int16_t, int);
__simd128_uint8_t __builtin_mve_vsriq_n_uv16qi(__simd128_uint8_t, __simd128_uint8_t, int);
__simd128_uint32_t __builtin_mve_vsriq_n_uv4si(__simd128_uint32_t, __simd128_uint32_t, int);
__simd128_uint16_t __builtin_mve_vsriq_n_uv8hi(__simd128_uint16_t, __simd128_uint16_t, int);
void __builtin_mve_vst1q_fv4sf(int*, __simd128_float32_t);
void __builtin_mve_vst1q_fv8hf(short int*, __simd128_float16_t);
void __builtin_mve_vst1q_sv16qi(signed char*, __simd128_int8_t);
void __builtin_mve_vst1q_sv4si(int*, __simd128_int32_t);
void __builtin_mve_vst1q_sv8hi(short int*, __simd128_int16_t);
void __builtin_mve_vst1q_uv16qi(signed char*, __simd128_uint8_t);
void __builtin_mve_vst1q_uv4si(int*, __simd128_uint32_t);
void __builtin_mve_vst1q_uv8hi(short int*, __simd128_uint16_t);
void __builtin_mve_vst2qv16qi(signed char*, __builtin_neon_oi);
void __builtin_mve_vst2qv4sf(float*, __builtin_neon_oi);
void __builtin_mve_vst2qv4si(int*, __builtin_neon_oi);
void __builtin_mve_vst2qv8hf(__bf16*, __builtin_neon_oi);
void __builtin_mve_vst2qv8hi(short int*, __builtin_neon_oi);
void __builtin_mve_vst4qv16qi(signed char*, __builtin_neon_xi);
void __builtin_mve_vst4qv4sf(float*, __builtin_neon_xi);
void __builtin_mve_vst4qv4si(int*, __builtin_neon_xi);
void __builtin_mve_vst4qv8hf(__bf16*, __builtin_neon_xi);
void __builtin_mve_vst4qv8hi(short int*, __builtin_neon_xi);
void __builtin_mve_vstrbq_p_sv16qi(signed char*, __simd128_int8_t, short unsigned int);
void __builtin_mve_vstrbq_p_sv4si(signed char*, __simd128_int32_t, short unsigned int);
void __builtin_mve_vstrbq_p_sv8hi(signed char*, __simd128_int16_t, short unsigned int);
void __builtin_mve_vstrbq_p_uv16qi(signed char*, __simd128_uint8_t, short unsigned int);
void __builtin_mve_vstrbq_p_uv4si(signed char*, __simd128_uint32_t, short unsigned int);
void __builtin_mve_vstrbq_p_uv8hi(signed char*, __simd128_uint16_t, short unsigned int);
void __builtin_mve_vstrbq_scatter_offset_p_sv16qi(signed char*, __simd128_uint8_t, __simd128_int8_t, short unsigned int);
void __builtin_mve_vstrbq_scatter_offset_p_sv4si(signed char*, __simd128_uint32_t, __simd128_int32_t, short unsigned int);
void __builtin_mve_vstrbq_scatter_offset_p_sv8hi(signed char*, __simd128_uint16_t, __simd128_int16_t, short unsigned int);
void __builtin_mve_vstrbq_scatter_offset_p_uv16qi(signed char*, __simd128_uint8_t, __simd128_uint8_t, short unsigned int);
void __builtin_mve_vstrbq_scatter_offset_p_uv4si(signed char*, __simd128_uint32_t, __simd128_uint32_t, short unsigned int);
void __builtin_mve_vstrbq_scatter_offset_p_uv8hi(signed char*, __simd128_uint16_t, __simd128_uint16_t, short unsigned int);
void __builtin_mve_vstrbq_scatter_offset_sv16qi(signed char*, __simd128_uint8_t, __simd128_int8_t);
void __builtin_mve_vstrbq_scatter_offset_sv4si(signed char*, __simd128_uint32_t, __simd128_int32_t);
void __builtin_mve_vstrbq_scatter_offset_sv8hi(signed char*, __simd128_uint16_t, __simd128_int16_t);
void __builtin_mve_vstrbq_scatter_offset_uv16qi(signed char*, __simd128_uint8_t, __simd128_uint8_t);
void __builtin_mve_vstrbq_scatter_offset_uv4si(signed char*, __simd128_uint32_t, __simd128_uint32_t);
void __builtin_mve_vstrbq_scatter_offset_uv8hi(signed char*, __simd128_uint16_t, __simd128_uint16_t);
void __builtin_mve_vstrbq_sv16qi(signed char*, __simd128_int8_t);
void __builtin_mve_vstrbq_sv4si(signed char*, __simd128_int32_t);
void __builtin_mve_vstrbq_sv8hi(signed char*, __simd128_int16_t);
void __builtin_mve_vstrbq_uv16qi(signed char*, __simd128_uint8_t);
void __builtin_mve_vstrbq_uv4si(signed char*, __simd128_uint32_t);
void __builtin_mve_vstrbq_uv8hi(signed char*, __simd128_uint16_t);
void __builtin_mve_vstrdq_scatter_base_p_sv2di(__simd128_uint64_t, int, __simd128_int64_t, short unsigned int);
void __builtin_mve_vstrdq_scatter_base_p_uv2di(__simd128_uint64_t, int, __simd128_uint64_t, short unsigned int);
void __builtin_mve_vstrdq_scatter_base_sv2di(__simd128_uint64_t, int, __simd128_int64_t);
void __builtin_mve_vstrdq_scatter_base_uv2di(__simd128_uint64_t, int, __simd128_uint64_t);
void __builtin_mve_vstrdq_scatter_base_wb_add_sv2di(__simd128_uint64_t, const int, __simd128_uint64_t);
void __builtin_mve_vstrdq_scatter_base_wb_add_uv2di(__simd128_uint64_t, const int, __simd128_uint64_t);
void __builtin_mve_vstrdq_scatter_base_wb_p_add_sv2di(__simd128_uint64_t, const int, __simd128_uint64_t, short unsigned int);
void __builtin_mve_vstrdq_scatter_base_wb_p_add_uv2di(__simd128_uint64_t, const int, __simd128_uint64_t, short unsigned int);
void __builtin_mve_vstrdq_scatter_base_wb_p_sv2di(__simd128_uint64_t, const int, __simd128_int64_t, short unsigned int);
void __builtin_mve_vstrdq_scatter_base_wb_p_uv2di(__simd128_uint64_t, const int, __simd128_uint64_t, short unsigned int);
void __builtin_mve_vstrdq_scatter_base_wb_sv2di(__simd128_uint64_t, const int, __simd128_int64_t);
void __builtin_mve_vstrdq_scatter_base_wb_uv2di(__simd128_uint64_t, const int, __simd128_uint64_t);
void __builtin_mve_vstrdq_scatter_offset_p_sv2di(long long int*, __simd128_uint64_t, __simd128_int64_t, short unsigned int);
void __builtin_mve_vstrdq_scatter_offset_p_uv2di(long long int*, __simd128_uint64_t, __simd128_uint64_t, short unsigned int);
void __builtin_mve_vstrdq_scatter_offset_sv2di(long long int*, __simd128_uint64_t, __simd128_int64_t);
void __builtin_mve_vstrdq_scatter_offset_uv2di(long long int*, __simd128_uint64_t, __simd128_uint64_t);
void __builtin_mve_vstrdq_scatter_shifted_offset_p_sv2di(long long int*, __simd128_uint64_t, __simd128_int64_t, short unsigned int);
void __builtin_mve_vstrdq_scatter_shifted_offset_p_uv2di(long long int*, __simd128_uint64_t, __simd128_uint64_t, short unsigned int);
void __builtin_mve_vstrdq_scatter_shifted_offset_sv2di(long long int*, __simd128_uint64_t, __simd128_int64_t);
void __builtin_mve_vstrdq_scatter_shifted_offset_uv2di(long long int*, __simd128_uint64_t, __simd128_uint64_t);
void __builtin_mve_vstrhq_fv8hf(short int*, __simd128_float16_t);
void __builtin_mve_vstrhq_p_fv8hf(short int*, __simd128_float16_t, short unsigned int);
void __builtin_mve_vstrhq_p_sv4si(short int*, __simd128_int32_t, short unsigned int);
void __builtin_mve_vstrhq_p_sv8hi(short int*, __simd128_int16_t, short unsigned int);
void __builtin_mve_vstrhq_p_uv4si(short int*, __simd128_uint32_t, short unsigned int);
void __builtin_mve_vstrhq_p_uv8hi(short int*, __simd128_uint16_t, short unsigned int);
void __builtin_mve_vstrhq_scatter_offset_fv8hf(short int*, __simd128_uint16_t, __simd128_float16_t);
void __builtin_mve_vstrhq_scatter_offset_p_fv8hf(short int*, __simd128_uint16_t, __simd128_float16_t, short unsigned int);
void __builtin_mve_vstrhq_scatter_offset_p_sv4si(short int*, __simd128_uint32_t, __simd128_int32_t, short unsigned int);
void __builtin_mve_vstrhq_scatter_offset_p_sv8hi(short int*, __simd128_uint16_t, __simd128_int16_t, short unsigned int);
void __builtin_mve_vstrhq_scatter_offset_p_uv4si(short int*, __simd128_uint32_t, __simd128_uint32_t, short unsigned int);
void __builtin_mve_vstrhq_scatter_offset_p_uv8hi(short int*, __simd128_uint16_t, __simd128_uint16_t, short unsigned int);
void __builtin_mve_vstrhq_scatter_offset_sv4si(short int*, __simd128_uint32_t, __simd128_int32_t);
void __builtin_mve_vstrhq_scatter_offset_sv8hi(short int*, __simd128_uint16_t, __simd128_int16_t);
void __builtin_mve_vstrhq_scatter_offset_uv4si(short int*, __simd128_uint32_t, __simd128_uint32_t);
void __builtin_mve_vstrhq_scatter_offset_uv8hi(short int*, __simd128_uint16_t, __simd128_uint16_t);
void __builtin_mve_vstrhq_scatter_shifted_offset_fv8hf(short int*, __simd128_uint16_t, __simd128_float16_t);
void __builtin_mve_vstrhq_scatter_shifted_offset_p_fv8hf(short int*, __simd128_uint16_t, __simd128_float16_t, short unsigned int);
void __builtin_mve_vstrhq_scatter_shifted_offset_p_sv4si(short int*, __simd128_uint32_t, __simd128_int32_t, short unsigned int);
void __builtin_mve_vstrhq_scatter_shifted_offset_p_sv8hi(short int*, __simd128_uint16_t, __simd128_int16_t, short unsigned int);
void __builtin_mve_vstrhq_scatter_shifted_offset_p_uv4si(short int*, __simd128_uint32_t, __simd128_uint32_t, short unsigned int);
void __builtin_mve_vstrhq_scatter_shifted_offset_p_uv8hi(short int*, __simd128_uint16_t, __simd128_uint16_t, short unsigned int);
void __builtin_mve_vstrhq_scatter_shifted_offset_sv4si(short int*, __simd128_uint32_t, __simd128_int32_t);
void __builtin_mve_vstrhq_scatter_shifted_offset_sv8hi(short int*, __simd128_uint16_t, __simd128_int16_t);
void __builtin_mve_vstrhq_scatter_shifted_offset_uv4si(short int*, __simd128_uint32_t, __simd128_uint32_t);
void __builtin_mve_vstrhq_scatter_shifted_offset_uv8hi(short int*, __simd128_uint16_t, __simd128_uint16_t);
void __builtin_mve_vstrhq_sv4si(short int*, __simd128_int32_t);
void __builtin_mve_vstrhq_sv8hi(short int*, __simd128_int16_t);
void __builtin_mve_vstrhq_uv4si(short int*, __simd128_uint32_t);
void __builtin_mve_vstrhq_uv8hi(short int*, __simd128_uint16_t);
void __builtin_mve_vstrwq_fv4sf(int*, __simd128_float32_t);
void __builtin_mve_vstrwq_p_fv4sf(int*, __simd128_float32_t, short unsigned int);
void __builtin_mve_vstrwq_p_sv4si(int*, __simd128_int32_t, short unsigned int);
void __builtin_mve_vstrwq_p_uv4si(int*, __simd128_uint32_t, short unsigned int);
void __builtin_mve_vstrwq_scatter_base_fv4sf(__simd128_uint32_t, int, __simd128_float32_t);
void __builtin_mve_vstrwq_scatter_base_p_fv4sf(__simd128_uint32_t, int, __simd128_float32_t, short unsigned int);
void __builtin_mve_vstrwq_scatter_base_p_sv4si(__simd128_uint32_t, int, __simd128_int32_t, short unsigned int);
void __builtin_mve_vstrwq_scatter_base_p_uv4si(__simd128_uint32_t, int, __simd128_uint32_t, short unsigned int);
void __builtin_mve_vstrwq_scatter_base_sv4si(__simd128_uint32_t, int, __simd128_int32_t);
void __builtin_mve_vstrwq_scatter_base_uv4si(__simd128_uint32_t, int, __simd128_uint32_t);
void __builtin_mve_vstrwq_scatter_base_wb_add_fv4sf(__simd128_uint32_t, const int, __simd128_uint32_t);
void __builtin_mve_vstrwq_scatter_base_wb_add_sv4si(__simd128_uint32_t, const int, __simd128_uint32_t);
void __builtin_mve_vstrwq_scatter_base_wb_add_uv4si(__simd128_uint32_t, const int, __simd128_uint32_t);
void __builtin_mve_vstrwq_scatter_base_wb_fv4sf(__simd128_uint32_t, const int, __simd128_float32_t);
void __builtin_mve_vstrwq_scatter_base_wb_p_add_fv4sf(__simd128_uint32_t, const int, __simd128_uint32_t, short unsigned int);
void __builtin_mve_vstrwq_scatter_base_wb_p_add_sv4si(__simd128_uint32_t, const int, __simd128_uint32_t, short unsigned int);
void __builtin_mve_vstrwq_scatter_base_wb_p_add_uv4si(__simd128_uint32_t, const int, __simd128_uint32_t, short unsigned int);
void __builtin_mve_vstrwq_scatter_base_wb_p_fv4sf(__simd128_uint32_t, const int, __simd128_float32_t, short unsigned int);
void __builtin_mve_vstrwq_scatter_base_wb_p_sv4si(__simd128_uint32_t, const int, __simd128_int32_t, short unsigned int);
void __builtin_mve_vstrwq_scatter_base_wb_p_uv4si(__simd128_uint32_t, const int, __simd128_uint32_t, short unsigned int);
void __builtin_mve_vstrwq_scatter_base_wb_sv4si(__simd128_uint32_t, const int, __simd128_int32_t);
void __builtin_mve_vstrwq_scatter_base_wb_uv4si(__simd128_uint32_t, const int, __simd128_uint32_t);
void __builtin_mve_vstrwq_scatter_offset_fv4sf(int*, __simd128_uint32_t, __simd128_float32_t);
void __builtin_mve_vstrwq_scatter_offset_p_fv4sf(int*, __simd128_uint32_t, __simd128_float32_t, short unsigned int);
void __builtin_mve_vstrwq_scatter_offset_p_sv4si(int*, __simd128_uint32_t, __simd128_int32_t, short unsigned int);
void __builtin_mve_vstrwq_scatter_offset_p_uv4si(int*, __simd128_uint32_t, __simd128_uint32_t, short unsigned int);
void __builtin_mve_vstrwq_scatter_offset_sv4si(int*, __simd128_uint32_t, __simd128_int32_t);
void __builtin_mve_vstrwq_scatter_offset_uv4si(int*, __simd128_uint32_t, __simd128_uint32_t);
void __builtin_mve_vstrwq_scatter_shifted_offset_fv4sf(int*, __simd128_uint32_t, __simd128_float32_t);
void __builtin_mve_vstrwq_scatter_shifted_offset_p_fv4sf(int*, __simd128_uint32_t, __simd128_float32_t, short unsigned int);
void __builtin_mve_vstrwq_scatter_shifted_offset_p_sv4si(int*, __simd128_uint32_t, __simd128_int32_t, short unsigned int);
void __builtin_mve_vstrwq_scatter_shifted_offset_p_uv4si(int*, __simd128_uint32_t, __simd128_uint32_t, short unsigned int);
void __builtin_mve_vstrwq_scatter_shifted_offset_sv4si(int*, __simd128_uint32_t, __simd128_int32_t);
void __builtin_mve_vstrwq_scatter_shifted_offset_uv4si(int*, __simd128_uint32_t, __simd128_uint32_t);
void __builtin_mve_vstrwq_sv4si(int*, __simd128_int32_t);
void __builtin_mve_vstrwq_uv4si(int*, __simd128_uint32_t);
__simd128_float32_t __builtin_mve_vsubq_fv4sf(__simd128_float32_t, __simd128_float32_t);
__simd128_float16_t __builtin_mve_vsubq_fv8hf(__simd128_float16_t, __simd128_float16_t);
__simd128_float32_t __builtin_mve_vsubq_m_fv4sf(__simd128_float32_t, __simd128_float32_t, __simd128_float32_t, short unsigned int);
__simd128_float16_t __builtin_mve_vsubq_m_fv8hf(__simd128_float16_t, __simd128_float16_t, __simd128_float16_t, short unsigned int);
__simd128_float32_t __builtin_mve_vsubq_m_n_fv4sf(__simd128_float32_t, __simd128_float32_t, float, short unsigned int);
__simd128_float16_t __builtin_mve_vsubq_m_n_fv8hf(__simd128_float16_t, __simd128_float16_t, __bf16, short unsigned int);
__simd128_int8_t __builtin_mve_vsubq_m_n_sv16qi(__simd128_int8_t, __simd128_int8_t, signed char, short unsigned int);
__simd128_int32_t __builtin_mve_vsubq_m_n_sv4si(__simd128_int32_t, __simd128_int32_t, int, short unsigned int);
__simd128_int16_t __builtin_mve_vsubq_m_n_sv8hi(__simd128_int16_t, __simd128_int16_t, short int, short unsigned int);
__simd128_uint8_t __builtin_mve_vsubq_m_n_uv16qi(__simd128_uint8_t, __simd128_uint8_t, unsigned char, short unsigned int);
__simd128_uint32_t __builtin_mve_vsubq_m_n_uv4si(__simd128_uint32_t, __simd128_uint32_t, unsigned int, short unsigned int);
__simd128_uint16_t __builtin_mve_vsubq_m_n_uv8hi(__simd128_uint16_t, __simd128_uint16_t, short unsigned int, short unsigned int);
__simd128_int8_t __builtin_mve_vsubq_m_sv16qi(__simd128_int8_t, __simd128_int8_t, __simd128_int8_t, short unsigned int);
__simd128_int32_t __builtin_mve_vsubq_m_sv4si(__simd128_int32_t, __simd128_int32_t, __simd128_int32_t, short unsigned int);
__simd128_int16_t __builtin_mve_vsubq_m_sv8hi(__simd128_int16_t, __simd128_int16_t, __simd128_int16_t, short unsigned int);
__simd128_uint8_t __builtin_mve_vsubq_m_uv16qi(__simd128_uint8_t, __simd128_uint8_t, __simd128_uint8_t, short unsigned int);
__simd128_uint32_t __builtin_mve_vsubq_m_uv4si(__simd128_uint32_t, __simd128_uint32_t, __simd128_uint32_t, short unsigned int);
__simd128_uint16_t __builtin_mve_vsubq_m_uv8hi(__simd128_uint16_t, __simd128_uint16_t, __simd128_uint16_t, short unsigned int);
__simd128_float32_t __builtin_mve_vsubq_n_fv4sf(__simd128_float32_t, float);
__simd128_float16_t __builtin_mve_vsubq_n_fv8hf(__simd128_float16_t, __bf16);
__simd128_int8_t __builtin_mve_vsubq_n_sv16qi(__simd128_int8_t, signed char);
__simd128_int32_t __builtin_mve_vsubq_n_sv4si(__simd128_int32_t, int);
__simd128_int16_t __builtin_mve_vsubq_n_sv8hi(__simd128_int16_t, short int);
__simd128_uint8_t __builtin_mve_vsubq_n_uv16qi(__simd128_uint8_t, unsigned char);
__simd128_uint32_t __builtin_mve_vsubq_n_uv4si(__simd128_uint32_t, unsigned int);
__simd128_uint16_t __builtin_mve_vsubq_n_uv8hi(__simd128_uint16_t, short unsigned int);
__simd128_int8_t __builtin_mve_vsubq_sv16qi(__simd128_int8_t, __simd128_int8_t);
__simd128_int32_t __builtin_mve_vsubq_sv4si(__simd128_int32_t, __simd128_int32_t);
__simd128_int16_t __builtin_mve_vsubq_sv8hi(__simd128_int16_t, __simd128_int16_t);
__simd128_uint8_t __builtin_mve_vsubq_uv16qi(__simd128_uint8_t, __simd128_uint8_t);
__simd128_uint32_t __builtin_mve_vsubq_uv4si(__simd128_uint32_t, __simd128_uint32_t);
__simd128_uint16_t __builtin_mve_vsubq_uv8hi(__simd128_uint16_t, __simd128_uint16_t);

#endif // __arm__

#ifdef __ARM_FEATURE_CMSE //eg, __ARM_FEATURE_CMSE is defined when passing -mcpu=cortex-m23 -mcmse
unsigned int __builtin_arm_cmse_nonsecure_caller();
#endif

#ifdef __ARM_NEON__
typedef signed char __builtin_neon_qi;
typedef short int __builtin_neon_hi;
typedef int __builtin_neon_si;
typedef long long int __builtin_neon_di;
typedef int __builtin_neon_ti __attribute__((mode(TI)));
typedef unsigned int __builtin_neon_uti __attribute__((mode(TI)));
typedef unsigned char __builtin_neon_uqi;
typedef short unsigned int __builtin_neon_uhi;
typedef unsigned int __builtin_neon_usi;
typedef unsigned long long __builtin_neon_udi;
typedef int __builtin_neon_ei __attribute__ ((__vector_size__(32)));
typedef int __builtin_neon_ci __attribute__ ((__vector_size__(64)));
typedef int __builtin_neon_poly8 __attribute__((mode(QI)));
typedef int __builtin_neon_poly16 __attribute__((mode(HI)));
typedef float __builtin_neon_sf;
typedef float __builtin_neon_hf;
typedef unsigned long long __builtin_neon_poly64;
typedef unsigned int __builtin_neon_poly128 __attribute__((mode(TI)));

typedef char __simd64_int8_t __attribute__((__vector_size__(8)));
typedef short __simd64_int16_t __attribute__((__vector_size__(8)));
typedef int __simd64_int32_t __attribute__((__vector_size__(8)));
typedef unsigned char __simd64_uint8_t __attribute__((__vector_size__(8)));
typedef unsigned short __simd64_uint16_t __attribute__((__vector_size__(8)));
typedef unsigned int __simd64_uint32_t __attribute__((__vector_size__(8)));

#ifdef __ARM_FEATURE_FP16_SCALAR_ARITHMETIC
typedef cov_fp16_t __fp16;
#endif
typedef cov_fp16_t __simd64_float16_t __attribute__((__vector_size__(8)));
typedef float __simd64_float32_t __attribute__((__vector_size__(8)));

typedef signed char __simd64_poly8_t __attribute__((__vector_size__(8)));
typedef short __simd64_poly16_t __attribute__((__vector_size__(8)));
typedef signed char __simd128_poly8_t __attribute__((__vector_size__(16)));
typedef short __simd128_poly16_t __attribute__((__vector_size__(16)));
typedef __bf16 __simd128_bfloat16_t __attribute__((__vector_size__(16)));
typedef __bf16 __simd64_bfloat16_t __attribute__((__vector_size__(8)));

__simd128_uint8_t __builtin_arm_crypto_aesd(__simd128_uint8_t, __simd128_uint8_t);
__simd128_uint8_t __builtin_arm_crypto_aese(__simd128_uint8_t, __simd128_uint8_t);
__simd128_uint8_t __builtin_arm_crypto_aesimc(__simd128_uint8_t);
__simd128_uint8_t __builtin_arm_crypto_aesmc(__simd128_uint8_t);
__simd128_uint32_t __builtin_arm_crypto_sha1c(__simd128_uint32_t, __simd128_uint32_t, __simd128_uint32_t);
__simd128_uint32_t __builtin_arm_crypto_sha1h(__simd128_uint32_t);
__simd128_uint32_t __builtin_arm_crypto_sha1m(__simd128_uint32_t, __simd128_uint32_t, __simd128_uint32_t);
__simd128_uint32_t __builtin_arm_crypto_sha1p(__simd128_uint32_t, __simd128_uint32_t, __simd128_uint32_t);
__simd128_uint32_t __builtin_arm_crypto_sha1su0(__simd128_uint32_t, __simd128_uint32_t, __simd128_uint32_t);
__simd128_uint32_t __builtin_arm_crypto_sha1su1(__simd128_uint32_t, __simd128_uint32_t);
__simd128_uint32_t __builtin_arm_crypto_sha256h(__simd128_uint32_t, __simd128_uint32_t, __simd128_uint32_t);
__simd128_uint32_t __builtin_arm_crypto_sha256h2(__simd128_uint32_t, __simd128_uint32_t, __simd128_uint32_t);
__simd128_uint32_t __builtin_arm_crypto_sha256su0(__simd128_uint32_t, __simd128_uint32_t);
__simd128_uint32_t __builtin_arm_crypto_sha256su1(__simd128_uint32_t, __simd128_uint32_t, __simd128_uint32_t);
__builtin_neon_uti __builtin_arm_crypto_vmullp64(long long unsigned int, long long unsigned int);
void __builtin_arm_lane_check(int, int);

__builtin_neon_ci __builtin_neon_vld3_lanev4sf (const __builtin_neon_sf * __a, __builtin_neon_ci __b, const int __c);
__builtin_neon_ci __builtin_neon_vld3_lanev4si (const __builtin_neon_si * __a, __builtin_neon_ci __b, const int __c);
__builtin_neon_ci __builtin_neon_vld3_lanev8hf(const cov_fp16_t*, __builtin_neon_ci, int);
__builtin_neon_ci __builtin_neon_vld3_lanev8hi (const __builtin_neon_hi * __a, __builtin_neon_ci __b, const int __c);
__builtin_neon_ci __builtin_neon_vld3v16qi (const __builtin_neon_qi *  __a);
__builtin_neon_ci __builtin_neon_vld3v4sf (const __builtin_neon_sf *  __a);
__builtin_neon_ci __builtin_neon_vld3v4si (const __builtin_neon_si *  __a);
__builtin_neon_ci __builtin_neon_vld3v8hf(const cov_fp16_t*);
__builtin_neon_ci __builtin_neon_vld3v8hi (const __builtin_neon_hi *  __a);
__builtin_neon_ci __builtin_neon_vld4_lanev4sf (const __builtin_neon_sf * __a, __builtin_neon_ci __b, const int __c);
__builtin_neon_ci __builtin_neon_vld4_lanev4si (const __builtin_neon_si * __a, __builtin_neon_ci __b, const int __c);
__builtin_neon_ci __builtin_neon_vld4_lanev8hi (const __builtin_neon_hi * __a, __builtin_neon_ci __b, const int __c);
__builtin_neon_ci __builtin_neon_vld4v16qi (const __builtin_neon_qi *  __a);
__builtin_neon_ci __builtin_neon_vld4v4sf (const __builtin_neon_sf *  __a);
__builtin_neon_ci __builtin_neon_vld4v4si (const __builtin_neon_si *  __a);
__builtin_neon_ci __builtin_neon_vld4v8hi (const __builtin_neon_hi *  __a);
__builtin_neon_ei __builtin_neon_vld3di (const __builtin_neon_di *  __a);
__builtin_neon_ei __builtin_neon_vld3_dupdi (const __builtin_neon_di *  __a);
__builtin_neon_ei __builtin_neon_vld3_dupv2sf (const __builtin_neon_sf *  __a);
__builtin_neon_ei __builtin_neon_vld3_dupv2si (const __builtin_neon_si *  __a);
__builtin_neon_ei __builtin_neon_vld3_dupv4hf(const cov_fp16_t*);
__builtin_neon_ei __builtin_neon_vld3_dupv4hi (const __builtin_neon_hi *  __a);
__builtin_neon_ei __builtin_neon_vld3_dupv8qi (const __builtin_neon_qi *  __a);
__builtin_neon_ei __builtin_neon_vld3_lanev2sf (const __builtin_neon_sf * __a, __builtin_neon_ei __b, const int __c);
__builtin_neon_ei __builtin_neon_vld3_lanev2si (const __builtin_neon_si * __a, __builtin_neon_ei __b, const int __c);
__builtin_neon_ei __builtin_neon_vld3_lanev4hf(const cov_fp16_t*, __builtin_neon_ei, int);
__builtin_neon_ei __builtin_neon_vld3_lanev4hi (const __builtin_neon_hi * __a, __builtin_neon_ei __b, const int __c);
__builtin_neon_ei __builtin_neon_vld3_lanev8qi (const __builtin_neon_qi * __a, __builtin_neon_ei __b, const int __c);
__builtin_neon_ei __builtin_neon_vld3v2sf (const __builtin_neon_sf *  __a);
__builtin_neon_ei __builtin_neon_vld3v2si (const __builtin_neon_si *  __a);
__builtin_neon_ei __builtin_neon_vld3v4hf(const cov_fp16_t*);
__builtin_neon_ei __builtin_neon_vld3v4hi (const __builtin_neon_hi *  __a);
__builtin_neon_ei __builtin_neon_vld3v8qi (const __builtin_neon_qi *  __a);
__builtin_neon_ei __builtin_neon_vld4di (const __builtin_neon_di *  __a);
__builtin_neon_ei __builtin_neon_vld4_dupdi (const __builtin_neon_di *  __a);
__builtin_neon_ei __builtin_neon_vld4_dupv2sf (const __builtin_neon_sf *  __a);
__builtin_neon_ei __builtin_neon_vld4_dupv2si (const __builtin_neon_si *  __a);
__builtin_neon_ei __builtin_neon_vld4_dupv4hi (const __builtin_neon_hi *  __a);
__builtin_neon_ei __builtin_neon_vld4_dupv8qi (const __builtin_neon_qi *  __a);
__builtin_neon_ei __builtin_neon_vld4_lanev2sf (const __builtin_neon_sf * __a, __builtin_neon_ei __b, const int __c);
__builtin_neon_ei __builtin_neon_vld4_lanev2si (const __builtin_neon_si * __a, __builtin_neon_ei __b, const int __c);
__builtin_neon_ei __builtin_neon_vld4_lanev4hi (const __builtin_neon_hi * __a, __builtin_neon_ei __b, const int __c);
__builtin_neon_ei __builtin_neon_vld4_lanev8qi (const __builtin_neon_qi * __a, __builtin_neon_ei __b, const int __c);
__builtin_neon_ei __builtin_neon_vld4v2sf (const __builtin_neon_sf *  __a);
__builtin_neon_ei __builtin_neon_vld4v2si (const __builtin_neon_si *  __a);
__builtin_neon_ei __builtin_neon_vld4v4hi (const __builtin_neon_hi *  __a);
__builtin_neon_ei __builtin_neon_vld4v8qi (const __builtin_neon_qi *  __a);
__builtin_neon_oi __builtin_neon_vld2_lanev4sf (const __builtin_neon_sf * __a, __builtin_neon_oi __b, const int __c);
__builtin_neon_oi __builtin_neon_vld2_lanev4si (const __builtin_neon_si * __a, __builtin_neon_oi __b, const int __c);
__builtin_neon_oi __builtin_neon_vld2_lanev8hf(const cov_fp16_t*, __builtin_neon_oi, int);
__builtin_neon_oi __builtin_neon_vld2_lanev8hi (const __builtin_neon_hi * __a, __builtin_neon_oi __b, const int __c);
__builtin_neon_oi __builtin_neon_vld2v16qi (const __builtin_neon_qi *  __a);
__builtin_neon_oi __builtin_neon_vld2v4sf (const __builtin_neon_sf *  __a);
__builtin_neon_oi __builtin_neon_vld2v4si (const __builtin_neon_si *  __a);
__builtin_neon_oi __builtin_neon_vld2v8hf(const cov_fp16_t*);
__builtin_neon_oi __builtin_neon_vld2v8hi (const __builtin_neon_hi *  __a);
__builtin_neon_oi __builtin_neon_vld4_dupv4hf(const cov_fp16_t*);
__builtin_neon_oi __builtin_neon_vld4_lanev4hf(const cov_fp16_t*, __builtin_neon_oi, int);
__builtin_neon_oi __builtin_neon_vld4v4hf(const cov_fp16_t*);
__builtin_neon_ti __builtin_neon_vld2di (const __builtin_neon_di *  __a);
__builtin_neon_ti __builtin_neon_vld2_dupdi (const __builtin_neon_di *  __a);
__builtin_neon_ti __builtin_neon_vld2_dupv2sf (const __builtin_neon_sf *  __a);
__builtin_neon_ti __builtin_neon_vld2_dupv2si (const __builtin_neon_si *  __a);
__builtin_neon_ti __builtin_neon_vld2_dupv4hf(const cov_fp16_t*);
__builtin_neon_ti __builtin_neon_vld2_dupv4hi (const __builtin_neon_hi *  __a);
__builtin_neon_ti __builtin_neon_vld2_dupv8qi (const __builtin_neon_qi *  __a);
__builtin_neon_ti __builtin_neon_vld2_lanev2sf (const __builtin_neon_sf * __a, float __b, const int __c);
__builtin_neon_ti __builtin_neon_vld2_lanev2si (const __builtin_neon_si * __a, int __b, const int __c);
__builtin_neon_ti __builtin_neon_vld2_lanev4hf(const cov_fp16_t*, __builtin_neon_ti, int);
__builtin_neon_ti __builtin_neon_vld2_lanev4hi (const __builtin_neon_hi * __a, int __b, const int __c);
__builtin_neon_ti __builtin_neon_vld2_lanev8qi (const __builtin_neon_qi * __a, int __b, const int __c);
__builtin_neon_ti __builtin_neon_vld2v2sf (const __builtin_neon_sf *  __a);
__builtin_neon_ti __builtin_neon_vld2v2si (const __builtin_neon_si *  __a);
__builtin_neon_ti __builtin_neon_vld2v4hf(const cov_fp16_t*);
__builtin_neon_ti __builtin_neon_vld2v4hi (const __builtin_neon_hi *  __a);
__builtin_neon_ti __builtin_neon_vld2v8qi (const __builtin_neon_qi *  __a);
__builtin_neon_uti __builtin_arm_crypto_vmullp64(long long unsigned int, long long unsigned int);
__builtin_neon_xi __builtin_neon_vld4_lanev8hf(const cov_fp16_t*, __builtin_neon_xi, int);
__builtin_neon_xi __builtin_neon_vld4v8hf(const cov_fp16_t*);
cov_fp16_t __builtin_neon_vabshf(cov_fp16_t);
cov_fp16_t __builtin_neon_vcvthshf(int);
cov_fp16_t __builtin_neon_vcvths_nhf(int, int);
cov_fp16_t __builtin_neon_vcvthuhf(int);
cov_fp16_t __builtin_neon_vcvthu_nhf(int, int);
cov_fp16_t __builtin_neon_vfmahf(cov_fp16_t, cov_fp16_t, cov_fp16_t);
cov_fp16_t __builtin_neon_vfmshf(cov_fp16_t, cov_fp16_t, cov_fp16_t);
cov_fp16_t __builtin_neon_vmaxnmhf(cov_fp16_t, cov_fp16_t);
cov_fp16_t __builtin_neon_vminnmhf(cov_fp16_t, cov_fp16_t);
cov_fp16_t __builtin_neon_vrndahf(cov_fp16_t);
cov_fp16_t __builtin_neon_vrndhf(cov_fp16_t);
cov_fp16_t __builtin_neon_vrndihf(cov_fp16_t);
cov_fp16_t __builtin_neon_vrndmhf(cov_fp16_t);
cov_fp16_t __builtin_neon_vrndnhf(cov_fp16_t);
cov_fp16_t __builtin_neon_vrndphf(cov_fp16_t);
cov_fp16_t __builtin_neon_vrndxhf(cov_fp16_t);
cov_fp16_t __builtin_neon_vsqrthf(cov_fp16_t);
float __builtin_neon_vget_lanev2sf (__simd64_float32_t __a, const int __b);
float __builtin_neon_vget_lanev4sf (__simd128_float32_t __a, const int __b);
int __builtin_neon_vbsldi (int  __a, int __b, int __c);
int __builtin_neon_vcreatedi (__builtin_neon_di __a);
int __builtin_neon_vcvtahssi(cov_fp16_t);
int __builtin_neon_vcvtahusi(cov_fp16_t);
int __builtin_neon_vcvths_nsi(cov_fp16_t, int);
int __builtin_neon_vcvthssi(cov_fp16_t);
int __builtin_neon_vcvthu_nsi(cov_fp16_t, int);
int __builtin_neon_vcvthusi(cov_fp16_t);
int __builtin_neon_vcvtmhssi(cov_fp16_t);
int __builtin_neon_vcvtmhusi(cov_fp16_t);
int __builtin_neon_vcvtnhssi(cov_fp16_t);
int __builtin_neon_vcvtnhusi(cov_fp16_t);
int __builtin_neon_vcvtphssi(cov_fp16_t);
int __builtin_neon_vcvtphusi(cov_fp16_t);
int __builtin_neon_vdup_lanedi (int __a, const int __b);
int __builtin_neon_vdup_ndi (__builtin_neon_di __a);
int __builtin_neon_vextdi (int __a, int __b, const int __c);
int __builtin_neon_vget_lanedi (int __a, const int __b);
int __builtin_neon_vget_laneuv16qi(__simd128_int8_t, int);
int __builtin_neon_vget_laneuv4hi(__simd64_int16_t, int);
int __builtin_neon_vget_laneuv8hi(__simd128_int16_t, int);
int __builtin_neon_vget_laneuv8qi(__simd64_int8_t, int);
int __builtin_neon_vget_lanev16qi(__simd128_int8_t, int);
int __builtin_neon_vget_lanev2si (__simd64_int32_t __a, const int __b);
int __builtin_neon_vget_lanev4hi(__simd64_int16_t, int);
int __builtin_neon_vget_lanev4si (__simd128_int32_t __a, const int __b);
int __builtin_neon_vget_lanev8hi(__simd128_int16_t, int);
int __builtin_neon_vget_lanev8qi(__simd64_int8_t, int);
int __builtin_neon_vld1di (const __builtin_neon_di *  __a);
int __builtin_neon_vld1_dupdi (const __builtin_neon_di *  __a);
int  __builtin_neon_vld1_lanedi (const __builtin_neon_di* __a, int __b, const int __c);
int __builtin_neon_vpadalsv2si (int __a, __simd64_int32_t __b);
int __builtin_neon_vpaddlsv2si (__simd64_int32_t __a);
int __builtin_neon_vqaddsdi (int __a, int __b);
int __builtin_neon_vqrshlsdi (int __a, int __b);
int __builtin_neon_vqshlsdi (int __a, int __b);
int __builtin_neon_vqshl_s_ndi (int __a, const int __b);
int __builtin_neon_vqsubsdi (int __a, int __b);
int __builtin_neon_vqsubudi (int  __a, int  __b);
int __builtin_neon_vreinterpretdidi (int  __a);
int __builtin_neon_vreinterpretdiv2sf (__simd64_float32_t __a);
int __builtin_neon_vreinterpretdiv2si (__simd64_int32_t  __a);
int __builtin_neon_vreinterpretdiv4hi (__simd64_int32_t __a);
int __builtin_neon_vreinterpretdiv8qi (__simd64_int32_t  __a);
int __builtin_neon_vrshlsdi (int __a, int __b);
int __builtin_neon_vrshrs_ndi (int __a, const int __b);
int __builtin_neon_vrsras_ndi (int __a, int __b, const int __c);
int __builtin_neon_vset_lanedi (__builtin_neon_di __a, int __b, const int __c);
int __builtin_neon_vshl_ndi (int __a, const int __b);
int __builtin_neon_vshlsdi (int __a, int __b);
int __builtin_neon_vshrs_ndi (int __a, const int __b);
int __builtin_neon_vsli_ndi (int __a, int __b, const int __c);
int __builtin_neon_vsras_ndi (int __a, int __b, const int __c);
int __builtin_neon_vsri_ndi (int __a, int __b, const int __c);
int __builtin_neon_vst1_lanedi (__builtin_neon_di * __a, int __b, const int __c);
void __builtin_neon_vst2di(long long int*, __builtin_neon_ti);
int __builtin_neon_vst2_lanev2sf (__builtin_neon_sf * __a, float __b, const int __c);
int __builtin_neon_vst2_lanev2si (__builtin_neon_si * __a, int __b, const int __c);
int __builtin_neon_vst2_lanev4hi (__builtin_neon_hi * __a, int __b, const int __c);
int __builtin_neon_vst2_lanev4sf (__builtin_neon_sf * __a, __builtin_neon_oi __b, const int __c);
int __builtin_neon_vst2_lanev4si (__builtin_neon_si * __a, __builtin_neon_oi __b, const int __c);
int __builtin_neon_vst2_lanev8hi (__builtin_neon_hi* __a, __builtin_neon_oi __b, const int __c);
int __builtin_neon_vst2_lanev8qi (__builtin_neon_qi * __a, int __b, const int __c);
void __builtin_neon_vst2v16qi(signed char*, __builtin_neon_oi);
void __builtin_neon_vst2v2sf(float*, __builtin_neon_ti);
void __builtin_neon_vst2v2si(int*, __builtin_neon_ti);
void __builtin_neon_vst2v4hi(short int*, __builtin_neon_ti);
void __builtin_neon_vst2v4sf(float*, __builtin_neon_oi);
void __builtin_neon_vst2v4si(int*, __builtin_neon_oi);
void __builtin_neon_vst2v8hi(short int*, __builtin_neon_oi);
void __builtin_neon_vst2v8qi(signed char*, __builtin_neon_ti);
void __builtin_neon_vst3di(long long int*, __builtin_neon_ei);
int __builtin_neon_vst3_lanev2sf (__builtin_neon_sf * __a, __builtin_neon_ei __b, const int __c);
int __builtin_neon_vst3_lanev2si (__builtin_neon_si * __a, __builtin_neon_ei __b, const int __c);
int __builtin_neon_vst3_lanev4hi (__builtin_neon_hi * __a, __builtin_neon_ei __b, const int __c);
int __builtin_neon_vst3_lanev4sf (__builtin_neon_sf * __a, __builtin_neon_ci __b, const int __c);
int __builtin_neon_vst3_lanev4si (__builtin_neon_si * __a, __builtin_neon_ci __b, const int __c);
int __builtin_neon_vst3_lanev8hi (__builtin_neon_hi* __a, __builtin_neon_ci __b, const int __c);
int __builtin_neon_vst3_lanev8qi (__builtin_neon_qi * __a, __builtin_neon_ei __b, const int __c);
void __builtin_neon_vst3v16qi(signed char*, __builtin_neon_ci);
void __builtin_neon_vst3v2sf(float*, __builtin_neon_ei);
void __builtin_neon_vst3v2si(int*, __builtin_neon_ei);
void __builtin_neon_vst3v4hi(short int*, __builtin_neon_ei);
void __builtin_neon_vst3v4sf(float*, __builtin_neon_ci);
void __builtin_neon_vst3v4si(int*, __builtin_neon_ci);
void __builtin_neon_vst3v8hi(short int*, __builtin_neon_ci);
void __builtin_neon_vst3v8qi(signed char*, __builtin_neon_ei);
void __builtin_neon_vst4di(long long int*, __builtin_neon_oi);
int __builtin_neon_vst4_lanev2sf (__builtin_neon_sf * __a, __builtin_neon_ei __b, const int __c);
int __builtin_neon_vst4_lanev2si (__builtin_neon_si * __a, __builtin_neon_ei __b, const int __c);
int __builtin_neon_vst4_lanev4hi (__builtin_neon_hi * __a, __builtin_neon_ei __b, const int __c);
int __builtin_neon_vst4_lanev4sf (__builtin_neon_sf * __a, __builtin_neon_ci __b, const int __c);
int __builtin_neon_vst4_lanev4si (__builtin_neon_si * __a, __builtin_neon_ci __b, const int __c);
int __builtin_neon_vst4_lanev8hi (__builtin_neon_hi* __a, __builtin_neon_ci __b, const int __c);
int __builtin_neon_vst4_lanev8qi (__builtin_neon_qi * __a, __builtin_neon_ei __b, const int __c);
void __builtin_neon_vst4v16qi(signed char*, __builtin_neon_xi);
void __builtin_neon_vst4v2sf(float*, __builtin_neon_oi);
void __builtin_neon_vst4v2si(int*, __builtin_neon_oi);
void __builtin_neon_vst4v4hi(short int*, __builtin_neon_oi);
void __builtin_neon_vst4v4sf(float*, __builtin_neon_xi);
void __builtin_neon_vst4v4si(int*, __builtin_neon_xi);
void __builtin_neon_vst4v8hi(short int*, __builtin_neon_xi);
void __builtin_neon_vst4v8qi(signed char*, __builtin_neon_oi);
long long int __builtin_neon_vget_highv2di(__simd128_int64_t);
long long int __builtin_neon_vget_lanev2di(__simd128_int64_t, int);
long long int __builtin_neon_vget_lowv2di(__simd128_int64_t);
__simd128_float16_t __builtin_neon_vabdv8hf(__simd128_float16_t, __simd128_float16_t);
__simd128_float16_t __builtin_neon_vabsv8hf(__simd128_float16_t);
__simd128_float16_t __builtin_neon_vaddv8hf(__simd128_float16_t, __simd128_float16_t);
__simd128_float16_t __builtin_neon_vbslv8hf(__simd128_int16_t, __simd128_float16_t, __simd128_float16_t);
__simd128_float16_t __builtin_neon_vcadd90v8hf(__simd128_float16_t, __simd128_float16_t);
__simd128_float16_t __builtin_neon_vcmla0v8hf(__simd128_float16_t, __simd128_float16_t, __simd128_float16_t);
__simd128_float16_t __builtin_neon_vcmla180v8hf(__simd128_float16_t, __simd128_float16_t, __simd128_float16_t);
__simd128_float16_t __builtin_neon_vcmla270v8hf(__simd128_float16_t, __simd128_float16_t, __simd128_float16_t);
__simd128_float16_t __builtin_neon_vcmla90v8hf(__simd128_float16_t, __simd128_float16_t, __simd128_float16_t);
__simd128_float16_t __builtin_neon_vcmla_lane0v8hf(__simd128_float16_t, __simd128_float16_t, __simd128_float16_t, int);
__simd128_float16_t __builtin_neon_vcmla_lane180v8hf(__simd128_float16_t, __simd128_float16_t, __simd128_float16_t, int);
__simd128_float16_t __builtin_neon_vcmla_lane270v8hf(__simd128_float16_t, __simd128_float16_t, __simd128_float16_t, int);
__simd128_float16_t __builtin_neon_vcmla_lane90v8hf(__simd128_float16_t, __simd128_float16_t, __simd128_float16_t, int);
__simd128_float16_t __builtin_neon_vcmlaq_lane0v8hf(__simd128_float16_t, __simd128_float16_t, __simd64_float16_t, int);
__simd128_float16_t __builtin_neon_vcmlaq_lane180v8hf(__simd128_float16_t, __simd128_float16_t, __simd64_float16_t, int);
__simd128_float16_t __builtin_neon_vcmlaq_lane270v8hf(__simd128_float16_t, __simd128_float16_t, __simd64_float16_t, int);
__simd128_float16_t __builtin_neon_vcmlaq_lane90v8hf(__simd128_float16_t, __simd128_float16_t, __simd64_float16_t, int);
__simd128_float16_t __builtin_neon_vcombinev4hf(__simd64_float16_t, __simd64_float16_t);
__simd128_float16_t __builtin_neon_vcvts_nv8hi(__simd128_int16_t, int);
__simd128_float16_t __builtin_neon_vcvtsv8hi(__simd128_int16_t);
__simd128_float16_t __builtin_neon_vcvtu_nv8hi(__simd128_int16_t, int);
__simd128_float16_t __builtin_neon_vcvtuv8hi(__simd128_int16_t);
__simd128_float16_t __builtin_neon_vdup_lanev8hf(__simd64_float16_t, int);
__simd128_float16_t __builtin_neon_vdup_nv8hf(cov_fp16_t);
__simd128_float16_t __builtin_neon_vextv8hf(__simd128_float16_t, __simd128_float16_t, int);
__simd128_float16_t __builtin_neon_vfmav8hf(__simd128_float16_t, __simd128_float16_t, __simd128_float16_t);
__simd128_float16_t __builtin_neon_vfmsv8hf(__simd128_float16_t, __simd128_float16_t, __simd128_float16_t);
__simd128_float16_t __builtin_neon_vld1v8hf(const cov_fp16_t*);
__simd128_float16_t __builtin_neon_vmaxfv8hf(__simd128_float16_t, __simd128_float16_t);
__simd128_float16_t __builtin_neon_vmaxnmv8hf(__simd128_float16_t, __simd128_float16_t);
__simd128_float16_t __builtin_neon_vminfv8hf(__simd128_float16_t, __simd128_float16_t);
__simd128_float16_t __builtin_neon_vminnmv8hf(__simd128_float16_t, __simd128_float16_t);
__simd128_float16_t __builtin_neon_vmulfv8hf(__simd128_float16_t, __simd128_float16_t);
__simd128_float16_t __builtin_neon_vmul_lanev8hf(__simd128_float16_t, __simd64_float16_t, int);
__simd128_float16_t __builtin_neon_vmul_nv8hf(__simd128_float16_t, cov_fp16_t);
__simd128_float16_t __builtin_neon_vnegv8hf(__simd128_float16_t);
__simd128_float16_t __builtin_neon_vrecpev8hf(__simd128_float16_t);
__simd128_float16_t __builtin_neon_vrecpsv8hf(__simd128_float16_t, __simd128_float16_t);
__simd128_float16_t __builtin_neon_vrndav8hf(__simd128_float16_t);
__simd128_float16_t __builtin_neon_vrndmv8hf(__simd128_float16_t);
__simd128_float16_t __builtin_neon_vrndnv8hf(__simd128_float16_t);
__simd128_float16_t __builtin_neon_vrndpv8hf(__simd128_float16_t);
__simd128_float16_t __builtin_neon_vrndv8hf(__simd128_float16_t);
__simd128_float16_t __builtin_neon_vrndxv8hf(__simd128_float16_t);
__simd128_float16_t __builtin_neon_vrsqrtev8hf(__simd128_float16_t);
__simd128_float16_t __builtin_neon_vrsqrtsv8hf(__simd128_float16_t, __simd128_float16_t);
__simd128_float16_t __builtin_neon_vsubv8hf(__simd128_float16_t, __simd128_float16_t);
__simd128_float32_t __builtin_neon_vabdfv4sf (__simd128_float32_t __a, __simd128_float32_t __b);
__simd128_float32_t __builtin_neon_vabsv4sf (__simd128_float32_t __a);
__simd128_float32_t __builtin_neon_vaddv4sf (__simd128_float32_t __a, __simd128_float32_t __b);
__simd128_float32_t __builtin_neon_vbslv4sf (__simd128_int32_t  __a, __simd128_float32_t __b, __simd128_float32_t __c);
__simd128_float32_t __builtin_neon_vcadd90v4sf(__simd128_float32_t, __simd128_float32_t);
__simd128_float32_t __builtin_neon_vcmla0v4sf(__simd128_float32_t, __simd128_float32_t, __simd128_float32_t);
__simd128_float32_t __builtin_neon_vcmla180v4sf(__simd128_float32_t, __simd128_float32_t, __simd128_float32_t);
__simd128_float32_t __builtin_neon_vcmla270v4sf(__simd128_float32_t, __simd128_float32_t, __simd128_float32_t);
__simd128_float32_t __builtin_neon_vcmla90v4sf(__simd128_float32_t, __simd128_float32_t, __simd128_float32_t);
__simd128_float32_t __builtin_neon_vcmla_lane0v4sf(__simd128_float32_t, __simd128_float32_t, __simd128_float32_t, int);
__simd128_float32_t __builtin_neon_vcmla_lane180v4sf(__simd128_float32_t, __simd128_float32_t, __simd128_float32_t, int);
__simd128_float32_t __builtin_neon_vcmla_lane270v4sf(__simd128_float32_t, __simd128_float32_t, __simd128_float32_t, int);
__simd128_float32_t __builtin_neon_vcmla_lane90v4sf(__simd128_float32_t, __simd128_float32_t, __simd128_float32_t, int);
__simd128_float32_t __builtin_neon_vcmlaq_lane0v4sf(__simd128_float32_t, __simd128_float32_t, __simd64_float32_t, int);
__simd128_float32_t __builtin_neon_vcmlaq_lane180v4sf(__simd128_float32_t, __simd128_float32_t, __simd64_float32_t, int);
__simd128_float32_t __builtin_neon_vcmlaq_lane270v4sf(__simd128_float32_t, __simd128_float32_t, __simd64_float32_t, int);
__simd128_float32_t __builtin_neon_vcmlaq_lane90v4sf(__simd128_float32_t, __simd128_float32_t, __simd64_float32_t, int);
__simd128_float32_t __builtin_neon_vcombinev2sf (__simd64_float32_t __a, __simd64_float32_t __b);
__simd128_float32_t __builtin_neon_vcvts_nv4si (__simd128_int32_t __a, const int __b);
__simd128_float32_t __builtin_neon_vcvtsv4si (__simd128_int32_t __a);
__simd128_float32_t __builtin_neon_vcvtu_nv4si (__simd128_int32_t  __a, const int __b);
__simd128_float32_t __builtin_neon_vcvtuv4si (__simd128_int32_t  __a);
__simd128_float32_t __builtin_neon_vcvtv4sfv4hf(__simd64_float16_t);
__simd128_float32_t __builtin_neon_vdup_lanev4sf (__simd64_float32_t __a, const int __b);
__simd128_float32_t __builtin_neon_vdup_nv4sf (__builtin_neon_sf __a);
__simd128_float32_t __builtin_neon_vextv4sf (__simd128_float32_t __a, __simd128_float32_t __b, const int __c);
__simd128_float32_t __builtin_neon_vfmal_highv4sf(__simd128_float32_t, __simd128_float16_t, __simd128_float16_t);
__simd128_float32_t __builtin_neon_vfmal_lane_highv4hfv4sf(__simd128_float32_t, __simd128_float16_t, __simd64_float16_t, int);
__simd128_float32_t __builtin_neon_vfmal_lane_highv4sf(__simd128_float32_t, __simd128_float16_t, __simd128_float16_t, int);
__simd128_float32_t __builtin_neon_vfmal_lane_lowv4hfv4sf(__simd128_float32_t, __simd128_float16_t, __simd64_float16_t, int);
__simd128_float32_t __builtin_neon_vfmal_lane_lowv4sf(__simd128_float32_t, __simd128_float16_t, __simd128_float16_t, int);
__simd128_float32_t __builtin_neon_vfmal_lowv4sf(__simd128_float32_t, __simd128_float16_t, __simd128_float16_t);
__simd128_float32_t __builtin_neon_vfmav4sf (__simd128_float32_t __a, __simd128_float32_t __b, __simd128_float32_t __c);
__simd128_float32_t __builtin_neon_vfmsl_highv4sf(__simd128_float32_t, __simd128_float16_t, __simd128_float16_t);
__simd128_float32_t __builtin_neon_vfmsl_lane_highv4hfv4sf(__simd128_float32_t, __simd128_float16_t, __simd64_float16_t, int);
__simd128_float32_t __builtin_neon_vfmsl_lane_highv4sf(__simd128_float32_t, __simd128_float16_t, __simd128_float16_t, int);
__simd128_float32_t __builtin_neon_vfmsl_lane_lowv4hfv4sf(__simd128_float32_t, __simd128_float16_t, __simd64_float16_t, int);
__simd128_float32_t __builtin_neon_vfmsl_lane_lowv4sf(__simd128_float32_t, __simd128_float16_t, __simd128_float16_t, int);
__simd128_float32_t __builtin_neon_vfmsl_lowv4sf(__simd128_float32_t, __simd128_float16_t, __simd128_float16_t);
__simd128_float32_t __builtin_neon_vfmsv4sf (__simd128_float32_t __a, __simd128_float32_t __b, __simd128_float32_t __c);
__simd128_float32_t __builtin_neon_vld1_dupv4sf (const __builtin_neon_sf *  __a);
__simd128_float32_t __builtin_neon_vld1_lanev4sf (const float * __a, __simd128_float32_t __b, const int __c);
__simd128_float32_t __builtin_neon_vld1v4sf (const __builtin_neon_sf *  __a);
__simd128_float32_t __builtin_neon_vmaxfv4sf (__simd128_float32_t __a, __simd128_float32_t __b);
__simd128_float32_t __builtin_neon_vmaxnmv4sf(__simd128_float32_t, __simd128_float32_t);
__simd128_float32_t __builtin_neon_vminfv4sf (__simd128_float32_t __a, __simd128_float32_t __b);
__simd128_float32_t __builtin_neon_vminnmv4sf(__simd128_float32_t, __simd128_float32_t);
__simd128_float32_t __builtin_neon_vmla_lanev4sf (__simd128_float32_t __a, __simd128_float32_t __b, __simd64_float32_t __c, const int __d);
__simd128_float32_t __builtin_neon_vmla_nv4sf (__simd128_float32_t __a, __simd128_float32_t __b, float __c);
__simd128_float32_t __builtin_neon_vmlav4sf (__simd128_float32_t __a, __simd128_float32_t __b, __simd128_float32_t __c);
__simd128_float32_t __builtin_neon_vmls_lanev4sf (__simd128_float32_t __a, __simd128_float32_t __b, __simd64_float32_t __c, const int __d);
__simd128_float32_t __builtin_neon_vmls_nv4sf (__simd128_float32_t __a, __simd128_float32_t __b, float __c);
__simd128_float32_t __builtin_neon_vmlsv4sf (__simd128_float32_t __a, __simd128_float32_t __b, __simd128_float32_t __c);
__simd128_float32_t __builtin_neon_vmulfv4sf (__simd128_float32_t __a, __simd128_float32_t __b);
__simd128_float32_t __builtin_neon_vmul_lanev4sf (__simd128_float32_t __a, __simd64_float32_t __b, const int __c);
__simd128_float32_t __builtin_neon_vmul_nv4sf (__simd128_float32_t __a, float __b);
__simd128_float32_t __builtin_neon_vnegv4sf (__simd128_float32_t __a);
__simd128_float32_t __builtin_neon_vrecpev4sf (__simd128_float32_t __a);
__simd128_float32_t __builtin_neon_vrecpsv4sf (__simd128_float32_t __a, __simd128_float32_t __b);
__simd128_float32_t __builtin_neon_vreinterpretv4sfti (__builtin_neon_ti __a);
__simd128_float32_t __builtin_neon_vreinterpretv4sfv16qi (__simd128_int32_t  __a);
__simd128_float32_t __builtin_neon_vreinterpretv4sfv2di (__simd128_int32_t  __a);
__simd128_float32_t __builtin_neon_vreinterpretv4sfv4si (__simd128_int32_t __a);
__simd128_float32_t __builtin_neon_vreinterpretv4sfv8hi (__simd128_int32_t  __a);
__simd128_float32_t __builtin_neon_vrintav4sf (__simd128_float32_t __a);
__simd128_float32_t __builtin_neon_vrintmv4sf (__simd128_float32_t __a);
__simd128_float32_t __builtin_neon_vrintnv4sf (__simd128_float32_t __a);
__simd128_float32_t __builtin_neon_vrintpv4sf (__simd128_float32_t __a);
__simd128_float32_t __builtin_neon_vrintxv4sf (__simd128_float32_t __a);
__simd128_float32_t __builtin_neon_vrintzv4sf (__simd128_float32_t __a);
__simd128_float32_t __builtin_neon_vrsqrtev4sf (__simd128_float32_t __a);
__simd128_float32_t __builtin_neon_vrsqrtsv4sf (__simd128_float32_t __a, __simd128_float32_t __b);
__simd128_float32_t __builtin_neon_vset_lanev4sf (float __a, __simd128_float32_t __b, const int __c);
void __builtin_neon_vst1_lanev4sf(float*, __simd128_float32_t, int);
void __builtin_neon_vst1v4sf(float*, __simd128_float32_t);
__simd128_float32_t __builtin_neon_vsubv4sf (__simd128_float32_t __a, __simd128_float32_t __b);
__simd128_int16_t __builtin_neon_vabalsv8qi(__simd128_int16_t, __simd64_int8_t, __simd64_int8_t);
__simd128_int16_t __builtin_neon_vabaluv8qi(__simd128_int16_t, __simd64_int8_t, __simd64_int8_t);
__simd128_int16_t __builtin_neon_vabasv8hi(__simd128_int16_t, __simd128_int16_t, __simd128_int16_t);
__simd128_int16_t __builtin_neon_vabauv8hi(__simd128_int16_t, __simd128_int16_t, __simd128_int16_t);
__simd128_int16_t __builtin_neon_vabdlsv8qi(__simd64_int8_t, __simd64_int8_t);
__simd128_int16_t __builtin_neon_vabdluv8qi(__simd64_int8_t, __simd64_int8_t);
__simd128_int16_t __builtin_neon_vabdsv8hi(__simd128_int16_t, __simd128_int16_t);
__simd128_int16_t __builtin_neon_vabduv8hi(__simd128_int16_t, __simd128_int16_t);
__simd128_int16_t __builtin_neon_vabsv8hi(__simd128_int16_t);
__simd128_int16_t __builtin_neon_vaddlsv8qi(__simd64_int8_t, __simd64_int8_t);
__simd128_int16_t __builtin_neon_vaddluv8qi(__simd64_int8_t, __simd64_int8_t);
__simd128_int16_t __builtin_neon_vaddwsv8qi(__simd128_int16_t, __simd64_int8_t);
__simd128_int16_t __builtin_neon_vaddwuv8qi(__simd128_int16_t, __simd64_int8_t);
__simd128_int16_t __builtin_neon_vbslv8hi(__simd128_int16_t, __simd128_int16_t, __simd128_int16_t);
__simd128_int16_t __builtin_neon_vcagev8hf(__simd128_float16_t, __simd128_float16_t);
__simd128_int16_t __builtin_neon_vcagtv8hf(__simd128_float16_t, __simd128_float16_t);
__simd128_int16_t __builtin_neon_vcalev8hf(__simd128_float16_t, __simd128_float16_t);
__simd128_int16_t __builtin_neon_vcaltv8hf(__simd128_float16_t, __simd128_float16_t);
__simd128_int16_t __builtin_neon_vceqv8hf(__simd128_float16_t, __simd128_float16_t);
__simd128_int16_t __builtin_neon_vceqv8hi(__simd128_int16_t, __simd128_int16_t);
__simd128_int16_t __builtin_neon_vceqzv8hf(__simd128_float16_t);
__simd128_int16_t __builtin_neon_vcgeuv8hi(__simd128_int16_t, __simd128_int16_t);
__simd128_int16_t __builtin_neon_vcgev8hf(__simd128_float16_t, __simd128_float16_t);
__simd128_int16_t __builtin_neon_vcgev8hi(__simd128_int16_t, __simd128_int16_t);
__simd128_int16_t __builtin_neon_vcgezv8hf(__simd128_float16_t);
__simd128_int16_t __builtin_neon_vcgtuv8hi(__simd128_int16_t, __simd128_int16_t);
__simd128_int16_t __builtin_neon_vcgtv8hf(__simd128_float16_t, __simd128_float16_t);
__simd128_int16_t __builtin_neon_vcgtv8hi(__simd128_int16_t, __simd128_int16_t);
__simd128_int16_t __builtin_neon_vcgtzv8hf(__simd128_float16_t);
__simd128_int16_t __builtin_neon_vclev8hf(__simd128_float16_t, __simd128_float16_t);
__simd128_int16_t __builtin_neon_vclezv8hf(__simd128_float16_t);
__simd128_int16_t __builtin_neon_vclsv8hi(__simd128_int16_t);
__simd128_int16_t __builtin_neon_vcltv8hf(__simd128_float16_t, __simd128_float16_t);
__simd128_int16_t __builtin_neon_vcltzv8hf(__simd128_float16_t);
__simd128_int16_t __builtin_neon_vclzv8hi(__simd128_int16_t);
__simd128_int16_t __builtin_neon_vcombinev4hi(__simd64_int16_t, __simd64_int16_t);
__simd128_int16_t __builtin_neon_vcvtasv8hf(__simd128_float16_t);
__simd128_int16_t __builtin_neon_vcvtauv8hf(__simd128_float16_t);
__simd128_int16_t __builtin_neon_vcvtmsv8hf(__simd128_float16_t);
__simd128_int16_t __builtin_neon_vcvtmuv8hf(__simd128_float16_t);
__simd128_int16_t __builtin_neon_vcvtnsv8hf(__simd128_float16_t);
__simd128_int16_t __builtin_neon_vcvtnuv8hf(__simd128_float16_t);
__simd128_int16_t __builtin_neon_vcvtpsv8hf(__simd128_float16_t);
__simd128_int16_t __builtin_neon_vcvtpuv8hf(__simd128_float16_t);
__simd128_int16_t __builtin_neon_vcvts_nv8hf(__simd128_float16_t, int);
__simd128_int16_t __builtin_neon_vcvtsv8hf(__simd128_float16_t);
__simd128_int16_t __builtin_neon_vcvtu_nv8hf(__simd128_float16_t, int);
__simd128_int16_t __builtin_neon_vcvtuv8hf(__simd128_float16_t);
__simd128_int16_t __builtin_neon_vdup_lanev8hi(__simd64_int16_t, int);
__simd128_int16_t __builtin_neon_vextv8hi(__simd128_int16_t, __simd128_int16_t, int);
__simd128_int16_t __builtin_neon_vhaddsv8hi(__simd128_int16_t, __simd128_int16_t);
__simd128_int16_t __builtin_neon_vhadduv8hi(__simd128_int16_t, __simd128_int16_t);
__simd128_int16_t __builtin_neon_vhsubsv8hi(__simd128_int16_t, __simd128_int16_t);
__simd128_int16_t __builtin_neon_vhsubuv8hi(__simd128_int16_t, __simd128_int16_t);
__simd128_int16_t __builtin_neon_vld1_lanev8hi(const short int*, __simd128_int16_t, int);
__simd128_int16_t __builtin_neon_vmaxsv8hi(__simd128_int16_t, __simd128_int16_t);
__simd128_int16_t __builtin_neon_vmaxuv8hi(__simd128_int16_t, __simd128_int16_t);
__simd128_int16_t __builtin_neon_vminsv8hi(__simd128_int16_t, __simd128_int16_t);
__simd128_int16_t __builtin_neon_vminuv8hi(__simd128_int16_t, __simd128_int16_t);
__simd128_int16_t __builtin_neon_vmla_lanev8hi(__simd128_int16_t, __simd128_int16_t, __simd64_int16_t, int);
__simd128_int16_t __builtin_neon_vmlalsv8qi(__simd128_int16_t, __simd64_int8_t, __simd64_int8_t);
__simd128_int16_t __builtin_neon_vmlaluv8qi(__simd128_int16_t, __simd64_int8_t, __simd64_int8_t);
__simd128_int16_t __builtin_neon_vmla_nv8hi(__simd128_int16_t, __simd128_int16_t, short int);
__simd128_int16_t __builtin_neon_vmlav8hi(__simd128_int16_t, __simd128_int16_t, __simd128_int16_t);
__simd128_int16_t __builtin_neon_vmls_lanev8hi(__simd128_int16_t, __simd128_int16_t, __simd64_int16_t, int);
__simd128_int16_t __builtin_neon_vmlslsv8qi(__simd128_int16_t, __simd64_int8_t, __simd64_int8_t);
__simd128_int16_t __builtin_neon_vmlsluv8qi(__simd128_int16_t, __simd64_int8_t, __simd64_int8_t);
__simd128_int16_t __builtin_neon_vmls_nv8hi(__simd128_int16_t, __simd128_int16_t, short int);
__simd128_int16_t __builtin_neon_vmlsv8hi(__simd128_int16_t, __simd128_int16_t, __simd128_int16_t);
__simd128_int16_t __builtin_neon_vmovlsv8qi(__simd64_int8_t);
__simd128_int16_t __builtin_neon_vmovluv8qi(__simd64_int8_t);
__simd128_int16_t __builtin_neon_vmul_lanev8hi(__simd128_int16_t, __simd64_int16_t, int);
__simd128_int16_t __builtin_neon_vmullpv8qi(__simd64_int8_t, __simd64_int8_t);
__simd128_int16_t __builtin_neon_vmullsv8qi(__simd64_int8_t, __simd64_int8_t);
__simd128_int16_t __builtin_neon_vmulluv8qi(__simd64_int8_t, __simd64_int8_t);
__simd128_int16_t __builtin_neon_vmul_nv8hi(__simd128_int16_t, short int);
__simd128_int16_t __builtin_neon_vmvnv8hi(__simd128_int16_t);
__simd128_int16_t __builtin_neon_vnegv8hi(__simd128_int16_t);
__simd128_int16_t __builtin_neon_vpadalsv16qi(__simd128_int16_t, __simd128_int8_t);
__simd128_int16_t __builtin_neon_vpadaluv16qi(__simd128_int16_t, __simd128_int8_t);
__simd128_int16_t __builtin_neon_vpaddlsv16qi(__simd128_int8_t);
__simd128_int16_t __builtin_neon_vpaddluv16qi(__simd128_int8_t);
__simd128_int16_t __builtin_neon_vqabsv8hi(__simd128_int16_t);
__simd128_int16_t __builtin_neon_vqaddsv8hi(__simd128_int16_t, __simd128_int16_t);
__simd128_int16_t __builtin_neon_vqadduv8hi(__simd128_int16_t, __simd128_int16_t);
__simd128_int16_t __builtin_neon_vqdmulh_lanev8hi(__simd128_int16_t, __simd64_int16_t, int);
__simd128_int16_t __builtin_neon_vqdmulh_nv8hi(__simd128_int16_t, short int);
__simd128_int16_t __builtin_neon_vqdmulhv8hi(__simd128_int16_t, __simd128_int16_t);
__simd128_int16_t __builtin_neon_vqnegv8hi(__simd128_int16_t);
__simd128_int16_t __builtin_neon_vqrdmlah_lanev8hi(__simd128_int16_t, __simd128_int16_t, __simd64_int16_t, int);
__simd128_int16_t __builtin_neon_vqrdmlahv8hi(__simd128_int16_t, __simd128_int16_t, __simd128_int16_t);
__simd128_int16_t __builtin_neon_vqrdmlsh_lanev8hi(__simd128_int16_t, __simd128_int16_t, __simd64_int16_t, int);
__simd128_int16_t __builtin_neon_vqrdmlshv8hi(__simd128_int16_t, __simd128_int16_t, __simd128_int16_t);
__simd128_int16_t __builtin_neon_vqrdmulh_lanev8hi(__simd128_int16_t, __simd64_int16_t, int);
__simd128_int16_t __builtin_neon_vqrdmulh_nv8hi(__simd128_int16_t, short int);
__simd128_int16_t __builtin_neon_vqrdmulhv8hi(__simd128_int16_t, __simd128_int16_t);
__simd128_int16_t __builtin_neon_vqrshlsv8hi(__simd128_int16_t, __simd128_int16_t);
__simd128_int16_t __builtin_neon_vqrshluv8hi(__simd128_int16_t, __simd128_int16_t);
__simd128_int16_t __builtin_neon_vqshl_s_nv8hi(__simd128_int16_t, int);
__simd128_int16_t __builtin_neon_vqshlsv8hi(__simd128_int16_t, __simd128_int16_t);
__simd128_int16_t __builtin_neon_vqshl_u_nv8hi(__simd128_int16_t, int);
__simd128_int16_t __builtin_neon_vqshlu_nv8hi(__simd128_int16_t, int);
__simd128_int16_t __builtin_neon_vqshluv8hi(__simd128_int16_t, __simd128_int16_t);
__simd128_int16_t __builtin_neon_vqsubsv8hi(__simd128_int16_t, __simd128_int16_t);
__simd128_int16_t __builtin_neon_vqsubuv8hi(__simd128_int16_t, __simd128_int16_t);
__simd128_int16_t __builtin_neon_vrhaddsv8hi(__simd128_int16_t, __simd128_int16_t);
__simd128_int16_t __builtin_neon_vrhadduv8hi(__simd128_int16_t, __simd128_int16_t);
__simd128_int16_t __builtin_neon_vrshlsv8hi(__simd128_int16_t, __simd128_int16_t);
__simd128_int16_t __builtin_neon_vrshluv8hi(__simd128_int16_t, __simd128_int16_t);
__simd128_int16_t __builtin_neon_vrshrs_nv8hi(__simd128_int16_t, int);
__simd128_int16_t __builtin_neon_vrshru_nv8hi(__simd128_int16_t, int);
__simd128_int16_t __builtin_neon_vrsras_nv8hi(__simd128_int16_t, __simd128_int16_t, int);
__simd128_int16_t __builtin_neon_vrsrau_nv8hi(__simd128_int16_t, __simd128_int16_t, int);
__simd128_int16_t __builtin_neon_vset_lanev8hi(short int, __simd128_int16_t, int);
__simd128_int16_t __builtin_neon_vshlls_nv8qi(__simd64_int8_t, int);
__simd128_int16_t __builtin_neon_vshllu_nv8qi(__simd64_int8_t, int);
__simd128_int16_t __builtin_neon_vshl_nv8hi(__simd128_int16_t, int);
__simd128_int16_t __builtin_neon_vshlsv8hi(__simd128_int16_t, __simd128_int16_t);
__simd128_int16_t __builtin_neon_vshluv8hi(__simd128_int16_t, __simd128_int16_t);
__simd128_int16_t __builtin_neon_vshrs_nv8hi(__simd128_int16_t, int);
__simd128_int16_t __builtin_neon_vshru_nv8hi(__simd128_int16_t, int);
__simd128_int16_t __builtin_neon_vsli_nv8hi(__simd128_int16_t, __simd128_int16_t, int);
__simd128_int16_t __builtin_neon_vsras_nv8hi(__simd128_int16_t, __simd128_int16_t, int);
__simd128_int16_t __builtin_neon_vsrau_nv8hi(__simd128_int16_t, __simd128_int16_t, int);
__simd128_int16_t __builtin_neon_vsri_nv8hi(__simd128_int16_t, __simd128_int16_t, int);
__simd128_int16_t __builtin_neon_vsublsv8qi(__simd64_int8_t, __simd64_int8_t);
__simd128_int16_t __builtin_neon_vsubluv8qi(__simd64_int8_t, __simd64_int8_t);
__simd128_int16_t __builtin_neon_vsubwsv8qi(__simd128_int16_t, __simd64_int8_t);
__simd128_int16_t __builtin_neon_vsubwuv8qi(__simd128_int16_t, __simd64_int8_t);
__simd128_int16_t __builtin_neon_vtstv8hi(__simd128_int16_t, __simd128_int16_t);
__simd128_int32_t __builtin_neon_sdot_lanev16qi(__simd128_int32_t, __simd128_int8_t, __simd64_int8_t, int);
__simd128_int32_t __builtin_neon_sdotv16qi(__simd128_int32_t, __simd128_int8_t, __simd128_int8_t);
__simd128_int32_t __builtin_neon_vabalsv4hi(__simd128_int32_t, __simd64_int16_t, __simd64_int16_t);
__simd128_int32_t __builtin_neon_vabaluv4hi(__simd128_int32_t, __simd64_int16_t, __simd64_int16_t);
__simd128_int32_t __builtin_neon_vabasv4si (__simd128_int32_t __a, __simd128_int32_t __b, __simd128_int32_t __c);
__simd128_int32_t __builtin_neon_vabauv4si (__simd128_int32_t  __a, __simd128_int32_t  __b, __simd128_int32_t  __c);
__simd128_int32_t __builtin_neon_vabdlsv2si (__simd64_int32_t __a, __simd64_int32_t __b);
__simd128_int32_t __builtin_neon_vabdlsv4hi(__simd64_int16_t, __simd64_int16_t);
__simd128_int32_t __builtin_neon_vabdluv2si (__simd64_int32_t  __a, __simd64_int32_t  __b);
__simd128_int32_t __builtin_neon_vabdluv4hi(__simd64_int16_t, __simd64_int16_t);
__simd128_int32_t __builtin_neon_vabdsv4si (__simd128_int32_t __a, __simd128_int32_t __b);
__simd128_int32_t __builtin_neon_vabduv4si (__simd128_int32_t  __a, __simd128_int32_t  __b);
__simd128_int32_t __builtin_neon_vabsv4si (__simd128_int32_t __a);
__simd128_int32_t __builtin_neon_vaddlsv2si (__simd64_int32_t __a, __simd64_int32_t __b);
__simd128_int32_t __builtin_neon_vaddlsv4hi(__simd64_int16_t, __simd64_int16_t);
__simd128_int32_t __builtin_neon_vaddluv2si (__simd64_int32_t  __a, __simd64_int32_t  __b);
__simd128_int32_t __builtin_neon_vaddluv4hi(__simd64_int16_t, __simd64_int16_t);
__simd128_int32_t __builtin_neon_vaddwsv4hi(__simd128_int32_t, __simd64_int16_t);
__simd128_int32_t __builtin_neon_vaddwuv4hi(__simd128_int32_t, __simd64_int16_t);
__simd128_int32_t __builtin_neon_vbslv4si (__simd128_int32_t __a, __simd128_int32_t __b, __simd128_int32_t __c);
__simd128_int32_t __builtin_neon_vcagev4sf (__simd128_float32_t __a, __simd128_float32_t __b);
__simd128_int32_t __builtin_neon_vcagtv4sf (__simd128_float32_t __a, __simd128_float32_t __b);
__simd128_int32_t __builtin_neon_vceqv4sf (__simd128_float32_t __a, __simd128_float32_t __b);
__simd128_int32_t __builtin_neon_vceqv4si (__simd128_int32_t __a, __simd128_int32_t __b);
__simd128_int32_t __builtin_neon_vcgeuv4si (__simd128_int32_t __a, __simd128_int32_t __b);
__simd128_int32_t __builtin_neon_vcgev4sf (__simd128_float32_t __a, __simd128_float32_t __b);
__simd128_int32_t __builtin_neon_vcgev4si (__simd128_int32_t __a, __simd128_int32_t __b);
__simd128_int32_t __builtin_neon_vcgtuv4si (__simd128_int32_t __a, __simd128_int32_t __b);
__simd128_int32_t __builtin_neon_vcgtv4sf (__simd128_float32_t __a, __simd128_float32_t __b);
__simd128_int32_t __builtin_neon_vcgtv4si (__simd128_int32_t __a, __simd128_int32_t __b);
__simd128_int32_t __builtin_neon_vclsv4si (__simd128_int32_t __a);
__simd128_int32_t __builtin_neon_vclzv4si (__simd128_int32_t __a);
__simd128_int64_t __builtin_neon_vcombinedi (long long int __a, long long int __b);
__simd128_int32_t __builtin_neon_vcombinev2si (__simd64_int32_t __a, __simd64_int32_t __b);
__simd128_int32_t __builtin_neon_vcvts_nv4sf (__simd128_float32_t __a, const int __b);
__simd128_int32_t __builtin_neon_vcvtsv4sf (__simd128_float32_t __a);
__simd128_int32_t __builtin_neon_vcvtu_nv4sf (__simd128_float32_t __a, const int __b);
__simd128_int32_t __builtin_neon_vcvtuv4sf (__simd128_float32_t __a);
__simd128_int32_t __builtin_neon_vdup_lanev2di (int __a, const int __b);
__simd128_int32_t __builtin_neon_vdup_lanev4si (__simd64_int32_t __a, const int __b);
__simd128_int8_t __builtin_neon_vdup_nv16qi(signed char);
__simd128_int64_t __builtin_neon_vdup_nv2di(long long int);
__simd128_int32_t __builtin_neon_vdup_nv4si (__builtin_neon_si __a);
__simd128_int16_t __builtin_neon_vdup_nv8hi(short int);
__simd128_int32_t __builtin_neon_vextv4si (__simd128_int32_t __a, __simd128_int32_t __b, const int __c);
__simd128_int32_t __builtin_neon_vhaddsv4si (__simd128_int32_t __a, __simd128_int32_t __b);
__simd128_int32_t __builtin_neon_vhadduv4si (__simd128_int32_t  __a, __simd128_int32_t  __b);
__simd128_int32_t __builtin_neon_vhsubsv4si (__simd128_int32_t __a, __simd128_int32_t __b);
__simd128_int32_t __builtin_neon_vhsubuv4si (__simd128_int32_t  __a, __simd128_int32_t  __b);
__simd128_int8_t __builtin_neon_vld1_dupv16qi(const signed char*);
__simd128_int64_t __builtin_neon_vld1_dupv2di(const long long int*);
__simd128_int32_t __builtin_neon_vld1_dupv4si (const __builtin_neon_si *  __a);
__simd128_int16_t __builtin_neon_vld1_dupv8hi(const short int*);
__simd128_int32_t __builtin_neon_vld1_lanev4si (const int * __a, __simd128_int32_t __b, const int __c);
__simd128_int8_t __builtin_neon_vld1v16qi(const signed char*);
__simd128_int64_t __builtin_neon_vld1v2di(const long long int*);
__simd128_int32_t __builtin_neon_vld1v4si (const __builtin_neon_si *  __a);
__simd128_int16_t __builtin_neon_vld1v8hi(const short int*);
__simd128_int32_t __builtin_neon_vmaxsv4si (__simd128_int32_t __a, __simd128_int32_t __b);
__simd128_int32_t __builtin_neon_vmaxuv4si (__simd128_int32_t  __a, __simd128_int32_t  __b);
__simd128_int32_t __builtin_neon_vminsv4si (__simd128_int32_t __a, __simd128_int32_t __b);
__simd128_int32_t __builtin_neon_vminuv4si (__simd128_int32_t  __a, __simd128_int32_t  __b);
__simd128_int32_t __builtin_neon_vmla_lanev4si (__simd128_int32_t __a, __simd128_int32_t __b, __simd64_int32_t __c, const int __d);
__simd128_int32_t __builtin_neon_vmlals_lanev4hi(__simd128_int32_t, __simd64_int16_t, __simd64_int16_t, int);
__simd128_int32_t __builtin_neon_vmlals_nv4hi(__simd128_int32_t, __simd64_int16_t, short int);
__simd128_int32_t __builtin_neon_vmlalsv4hi(__simd128_int32_t, __simd64_int16_t, __simd64_int16_t);
__simd128_int32_t __builtin_neon_vmlalu_lanev4hi(__simd128_int32_t, __simd64_int16_t, __simd64_int16_t, int);
__simd128_int32_t __builtin_neon_vmlalu_nv4hi(__simd128_int32_t, __simd64_int16_t, short int);
__simd128_int32_t __builtin_neon_vmlaluv4hi(__simd128_int32_t, __simd64_int16_t, __simd64_int16_t);
__simd128_int32_t __builtin_neon_vmla_nv4si (__simd128_int32_t __a, __simd128_int32_t __a, int __c);
__simd128_int32_t __builtin_neon_vmlav4si (__simd128_int32_t __a, __simd128_int32_t __b, __simd128_int32_t __c);
__simd128_int32_t __builtin_neon_vmls_lanev4si (__simd128_int32_t __a, __simd128_int32_t __b, __simd64_int32_t __c, const int __d);
__simd128_int32_t __builtin_neon_vmlsls_lanev4hi(__simd128_int32_t, __simd64_int16_t, __simd64_int16_t, int);
__simd128_int32_t __builtin_neon_vmlsls_nv4hi(__simd128_int32_t, __simd64_int16_t, short int);
__simd128_int32_t __builtin_neon_vmlslsv4hi(__simd128_int32_t, __simd64_int16_t, __simd64_int16_t);
__simd128_int32_t __builtin_neon_vmlslu_lanev4hi(__simd128_int32_t, __simd64_int16_t, __simd64_int16_t, int);
__simd128_int32_t __builtin_neon_vmlslu_nv4hi(__simd128_int32_t, __simd64_int16_t, short int);
__simd128_int32_t __builtin_neon_vmlsluv4hi(__simd128_int32_t, __simd64_int16_t, __simd64_int16_t);
__simd128_int32_t __builtin_neon_vmls_nv4si (__simd128_int32_t __a, __simd128_int32_t __a, int __c);
__simd128_int32_t __builtin_neon_vmlsv4si (__simd128_int32_t __a, __simd128_int32_t __b, __simd128_int32_t __c);
__simd128_int32_t __builtin_neon_vmovlsv2si (__simd64_int32_t __a);
__simd128_int32_t __builtin_neon_vmovlsv4hi(__simd64_int16_t);
__simd128_int32_t __builtin_neon_vmovluv2si (__simd64_int32_t  __a);
__simd128_int32_t __builtin_neon_vmovluv4hi(__simd64_int16_t);
__simd128_int32_t __builtin_neon_vmul_lanev4si (__simd128_int32_t __a, __simd64_int32_t __b, const int __c);
__simd128_int32_t __builtin_neon_vmulls_lanev2si (__simd64_int32_t __a, __simd64_int32_t __b, const int __c);
__simd128_int32_t __builtin_neon_vmulls_lanev4hi(__simd64_int16_t, __simd64_int16_t, int);
__simd128_int32_t __builtin_neon_vmulls_nv2si (__simd64_int32_t __a, int __b);
__simd128_int32_t __builtin_neon_vmulls_nv4hi(__simd64_int16_t, short int);
__simd128_int32_t __builtin_neon_vmullsv2si (__simd64_int32_t __a, __simd64_int32_t __b);
__simd128_int32_t __builtin_neon_vmullsv4hi(__simd64_int16_t, __simd64_int16_t);
__simd128_int32_t __builtin_neon_vmullu_lanev2si (__simd64_int32_t __a, __simd64_int32_t __b, const int __c);
__simd128_int32_t __builtin_neon_vmullu_lanev4hi(__simd64_int16_t, __simd64_int16_t, int);
__simd128_int32_t __builtin_neon_vmullu_nv2si (__simd64_int32_t  __a, __builtin_neon_si  __b);
__simd128_int32_t __builtin_neon_vmullu_nv4hi(__simd64_int16_t, short int);
__simd128_int32_t __builtin_neon_vmulluv2si (__simd64_int32_t  __a, __simd64_int32_t  __b);
__simd128_int32_t __builtin_neon_vmulluv4hi(__simd64_int16_t, __simd64_int16_t);
__simd128_int32_t __builtin_neon_vmul_nv4si (__simd128_int32_t __a, int __b);
__simd128_int32_t __builtin_neon_vmvnv4si (__simd128_int32_t __a);
__simd128_int32_t __builtin_neon_vnegv4si (__simd128_int32_t __a);
__simd128_int32_t __builtin_neon_vpadalsv8hi(__simd128_int32_t, __simd128_int16_t);
__simd128_int32_t __builtin_neon_vpadaluv8hi(__simd128_int32_t, __simd128_int16_t);
__simd128_int32_t __builtin_neon_vpaddlsv4si (__simd128_int32_t __a);
__simd128_int32_t __builtin_neon_vpaddlsv8hi(__simd128_int16_t);
__simd128_int32_t __builtin_neon_vpaddluv4si (__simd128_int32_t  __a);
__simd128_int32_t __builtin_neon_vpaddluv8hi(__simd128_int16_t);
__simd128_int32_t __builtin_neon_vqabsv4si (__simd128_int32_t __a);
__simd128_int32_t __builtin_neon_vqaddsv4si (__simd128_int32_t __a, __simd128_int32_t __b);
__simd128_int32_t __builtin_neon_vqadduv4si (__simd128_int32_t  __a, __simd128_int32_t  __b);
__simd128_int32_t __builtin_neon_vqdmlal_lanev4hi(__simd128_int32_t, __simd64_int16_t, __simd64_int16_t, int);
__simd128_int32_t __builtin_neon_vqdmlal_nv4hi(__simd128_int32_t, __simd64_int16_t, short int);
__simd128_int32_t __builtin_neon_vqdmlalv4hi(__simd128_int32_t, __simd64_int16_t, __simd64_int16_t);
__simd128_int32_t __builtin_neon_vqdmlsl_lanev4hi(__simd128_int32_t, __simd64_int16_t, __simd64_int16_t, int);
__simd128_int32_t __builtin_neon_vqdmlsl_nv4hi(__simd128_int32_t, __simd64_int16_t, short int);
__simd128_int32_t __builtin_neon_vqdmlslv4hi(__simd128_int32_t, __simd64_int16_t, __simd64_int16_t);
__simd128_int32_t __builtin_neon_vqdmulh_lanev4si (__simd128_int32_t __a, __simd64_int32_t __b, const int __c);
__simd128_int32_t __builtin_neon_vqdmulh_nv4si (__simd128_int32_t __a, int __b);
__simd128_int32_t __builtin_neon_vqdmulhv4si (__simd128_int32_t __a, __simd128_int32_t __b);
__simd128_int32_t __builtin_neon_vqdmull_lanev2si (__simd64_int32_t __a, __simd64_int32_t __b, const int __c);
__simd128_int32_t __builtin_neon_vqdmull_lanev4hi(__simd64_int16_t, __simd64_int16_t, int);
__simd128_int32_t __builtin_neon_vqdmull_nv2si (__simd64_int32_t __a, int __b);
__simd128_int32_t __builtin_neon_vqdmull_nv4hi(__simd64_int16_t, short int);
__simd128_int32_t __builtin_neon_vqdmullv2si (__simd64_int32_t __a, __simd64_int32_t __b);
__simd128_int32_t __builtin_neon_vqdmullv4hi(__simd64_int16_t, __simd64_int16_t);
__simd128_int32_t __builtin_neon_vqnegv4si (__simd128_int32_t __a);
__simd128_int32_t __builtin_neon_vqrdmlah_lanev4si(__simd128_int32_t, __simd128_int32_t, __simd64_int32_t, int);
__simd128_int32_t __builtin_neon_vqrdmlahv4si(__simd128_int32_t, __simd128_int32_t, __simd128_int32_t);
__simd128_int32_t __builtin_neon_vqrdmlsh_lanev4si(__simd128_int32_t, __simd128_int32_t, __simd64_int32_t, int);
__simd128_int32_t __builtin_neon_vqrdmlshv4si(__simd128_int32_t, __simd128_int32_t, __simd128_int32_t);
__simd128_int32_t __builtin_neon_vqrdmulh_lanev4si (__simd128_int32_t __a, __simd64_int32_t __b, const int __c);
__simd128_int32_t __builtin_neon_vqrdmulh_nv4si (__simd128_int32_t __a, int __b);
__simd128_int32_t __builtin_neon_vqrdmulhv4si (__simd128_int32_t __a, __simd128_int32_t __b);
__simd128_int32_t __builtin_neon_vqrshlsv4si (__simd128_int32_t __a, __simd128_int32_t __b);
__simd128_int32_t __builtin_neon_vqrshluv4si (__simd128_int32_t  __a, __simd128_int32_t __b);
__simd128_int32_t __builtin_neon_vqshl_s_nv4si (__simd128_int32_t __a, const int __b);
__simd128_int32_t __builtin_neon_vqshlsv4si (__simd128_int32_t __a, __simd128_int32_t __b);
__simd128_int32_t __builtin_neon_vqshl_u_nv4si (__simd128_int32_t  __a, const int __b);
__simd128_int32_t __builtin_neon_vqshlu_nv4si (__simd128_int32_t __a, const int __b);
__simd128_int32_t __builtin_neon_vqshluv4si (__simd128_int32_t  __a, __simd128_int32_t __b);
__simd128_int32_t __builtin_neon_vqsubsv4si (__simd128_int32_t __a, __simd128_int32_t __b);
__simd128_int32_t __builtin_neon_vqsubuv4si (__simd128_int32_t  __a, __simd128_int32_t  __b);
__simd128_int32_t __builtin_neon_vrecpev4si (__simd128_int32_t  __a);
__simd128_int32_t __builtin_neon_vreinterprettiv16qi (__simd128_int32_t __a);
__simd128_int32_t __builtin_neon_vreinterprettiv2di (__simd128_int32_t __a);
__simd128_int32_t __builtin_neon_vreinterprettiv4sf (__simd128_float32_t __a);
__simd128_int32_t __builtin_neon_vreinterprettiv4si (__simd128_int32_t __a);
__simd128_int32_t __builtin_neon_vreinterprettiv8hi (__simd128_int32_t __a);
__simd128_int32_t __builtin_neon_vreinterpretv16qiti (__builtin_neon_ti __a);
__simd128_int32_t __builtin_neon_vreinterpretv16qiv16qi (__simd128_int32_t __a);
__simd128_int32_t __builtin_neon_vreinterpretv16qiv2di (__simd128_int32_t  __a);
__simd128_int32_t __builtin_neon_vreinterpretv16qiv4sf (__simd128_float32_t __a);
__simd128_int32_t __builtin_neon_vreinterpretv16qiv4si (__simd128_int32_t __a);
__simd128_int32_t __builtin_neon_vreinterpretv16qiv8hi (__simd128_int32_t  __a);
__simd128_int32_t __builtin_neon_vreinterpretv2diti (__builtin_neon_ti __a);
__simd128_int32_t __builtin_neon_vreinterpretv2div16qi (__simd128_int32_t  __a);
__simd128_int32_t __builtin_neon_vreinterpretv2div2di (__simd128_int32_t __a);
__simd128_int32_t __builtin_neon_vreinterpretv2div4sf (__simd128_float32_t __a);
__simd128_int32_t __builtin_neon_vreinterpretv2div4si (__simd128_int32_t  __a);
__simd128_int32_t __builtin_neon_vreinterpretv2div8hi (__simd128_int32_t  __a);
__simd128_int32_t __builtin_neon_vreinterpretv4siti (__builtin_neon_ti __a);
__simd128_int32_t __builtin_neon_vreinterpretv4siv16qi (__simd128_int32_t  __a);
__simd128_int32_t __builtin_neon_vreinterpretv4siv2di (__simd128_int32_t  __a);
__simd128_int32_t __builtin_neon_vreinterpretv4siv4sf (__simd128_float32_t __a);
__simd128_int32_t __builtin_neon_vreinterpretv4siv4si (__simd128_int32_t  __a);
__simd128_int32_t __builtin_neon_vreinterpretv4siv8hi (__simd128_int32_t  __a);
__simd128_int32_t __builtin_neon_vreinterpretv8hiti (__builtin_neon_ti __a);
__simd128_int32_t __builtin_neon_vreinterpretv8hiv16qi (__simd128_int32_t  __a);
__simd128_int32_t __builtin_neon_vreinterpretv8hiv2di (__simd128_int32_t  __a);
__simd128_int32_t __builtin_neon_vreinterpretv8hiv4sf (__simd128_float32_t __a);
__simd128_int32_t __builtin_neon_vreinterpretv8hiv4si (__simd128_int32_t  __a);
__simd128_int32_t __builtin_neon_vreinterpretv8hiv8hi (__simd128_int32_t  __a);
__simd128_int32_t __builtin_neon_vrhaddsv4si (__simd128_int32_t __a, __simd128_int32_t __b);
__simd128_int32_t __builtin_neon_vrhadduv4si (__simd128_int32_t  __a, __simd128_int32_t  __b);
__simd128_int32_t __builtin_neon_vrshlsv4si (__simd128_int32_t __a, __simd128_int32_t __b);
__simd128_int32_t __builtin_neon_vrshluv4si (__simd128_int32_t  __a, __simd128_int32_t __b);
__simd128_int32_t __builtin_neon_vrshrs_nv4si (__simd128_int32_t __a, const int __b);
__simd128_int32_t __builtin_neon_vrshru_nv4si (__simd128_int32_t  __a, const int __b);
__simd128_int32_t __builtin_neon_vrsqrtev4si (__simd128_int32_t  __a);
__simd128_int32_t __builtin_neon_vrsras_nv4si (__simd128_int32_t __a, __simd128_int32_t __b, const int __c);
__simd128_int32_t __builtin_neon_vrsrau_nv4si (__simd128_int32_t  __a, __simd128_int32_t  __b, const int __c);
__simd128_int32_t __builtin_neon_vset_lanev4si (int __a, __simd128_int32_t __a, const int __c);
__simd128_int32_t __builtin_neon_vshlls_nv2si (__simd64_int32_t __a, const int __b);
__simd128_int32_t __builtin_neon_vshlls_nv4hi(__simd64_int16_t, int);
__simd128_int32_t __builtin_neon_vshllu_nv2si (__simd64_int32_t __a, const int __b);
__simd128_int32_t __builtin_neon_vshllu_nv4hi(__simd64_int16_t, int);
__simd128_int32_t __builtin_neon_vshl_nv4si (__simd128_int32_t __a, const int __b);
__simd128_int32_t __builtin_neon_vshlsv4si (__simd128_int32_t __a, __simd128_int32_t __b);
__simd128_int32_t __builtin_neon_vshluv4si (__simd128_int32_t  __a, __simd128_int32_t __b);
__simd128_int32_t __builtin_neon_vshrs_nv4si (__simd128_int32_t __a, const int __b);
__simd128_int32_t __builtin_neon_vshru_nv4si (__simd128_int32_t  __a, const int __b);
__simd128_int32_t __builtin_neon_vsli_nv4si (__simd128_int32_t __a, __simd128_int32_t __b, const int __c);
__simd128_int32_t __builtin_neon_vsras_nv4si (__simd128_int32_t __a, __simd128_int32_t __b, const int __c);
__simd128_int32_t __builtin_neon_vsrau_nv4si (__simd128_int32_t  __a, __simd128_int32_t  __b, const int __c);
__simd128_int32_t __builtin_neon_vsri_nv4si (__simd128_int32_t __a, __simd128_int32_t __b, const int __c);
void __builtin_neon_vst1_lanev4si(int*, __simd128_int32_t, int);
void __builtin_neon_vst1v4si(int*, __simd128_int32_t);
__simd128_int32_t __builtin_neon_vsublsv2si (__simd64_int32_t __a, __simd64_int32_t __b);
__simd128_int32_t __builtin_neon_vsublsv4hi(__simd64_int16_t, __simd64_int16_t);
__simd128_int32_t __builtin_neon_vsubluv2si (__simd64_int32_t  __a, __simd64_int32_t  __b);
__simd128_int32_t __builtin_neon_vsubluv4hi(__simd64_int16_t, __simd64_int16_t);
__simd128_int32_t __builtin_neon_vsubwsv4hi(__simd128_int32_t, __simd64_int16_t);
__simd128_int32_t __builtin_neon_vsubwuv4hi(__simd128_int32_t, __simd64_int16_t);
__simd128_int32_t __builtin_neon_vtstv4si (__simd128_int32_t __a, __simd128_int32_t __b);
__simd128_int64_t __builtin_neon_vabalsv2si(__simd128_int64_t, __simd64_int32_t, __simd64_int32_t);
__simd128_int64_t __builtin_neon_vabaluv2si(__simd128_int64_t, __simd64_int32_t, __simd64_int32_t);
__simd128_int64_t __builtin_neon_vaddwsv2si(__simd128_int64_t, __simd64_int32_t);
__simd128_int64_t __builtin_neon_vaddwuv2si(__simd128_int64_t, __simd64_int32_t);
__simd128_int64_t __builtin_neon_vbslv2di(__simd128_int64_t, __simd128_int64_t, __simd128_int64_t);
__simd128_int64_t __builtin_neon_vextv2di(__simd128_int64_t, __simd128_int64_t, int);
__simd128_int64_t __builtin_neon_vld1_lanev2di(const long long int*, __simd128_int64_t, int);
__simd128_int64_t __builtin_neon_vmlals_lanev2si(__simd128_int64_t, __simd64_int32_t, __simd64_int32_t, int);
__simd128_int64_t __builtin_neon_vmlals_nv2si(__simd128_int64_t, __simd64_int32_t, int);
__simd128_int64_t __builtin_neon_vmlalsv2si(__simd128_int64_t, __simd64_int32_t, __simd64_int32_t);
__simd128_int64_t __builtin_neon_vmlalu_lanev2si(__simd128_int64_t, __simd64_int32_t, __simd64_int32_t, int);
__simd128_int64_t __builtin_neon_vmlalu_nv2si(__simd128_int64_t, __simd64_int32_t, int);
__simd128_int64_t __builtin_neon_vmlaluv2si(__simd128_int64_t, __simd64_int32_t, __simd64_int32_t);
__simd128_int64_t __builtin_neon_vmlsls_lanev2si(__simd128_int64_t, __simd64_int32_t, __simd64_int32_t, int);
__simd128_int64_t __builtin_neon_vmlsls_nv2si(__simd128_int64_t, __simd64_int32_t, int);
__simd128_int64_t __builtin_neon_vmlslsv2si(__simd128_int64_t, __simd64_int32_t, __simd64_int32_t);
__simd128_int64_t __builtin_neon_vmlslu_lanev2si(__simd128_int64_t, __simd64_int32_t, __simd64_int32_t, int);
__simd128_int64_t __builtin_neon_vmlslu_nv2si(__simd128_int64_t, __simd64_int32_t, int);
__simd128_int64_t __builtin_neon_vmlsluv2si(__simd128_int64_t, __simd64_int32_t, __simd64_int32_t);
__simd128_int64_t __builtin_neon_vpadalsv4si(__simd128_int64_t, __simd128_int32_t);
__simd128_int64_t __builtin_neon_vpadaluv4si(__simd128_int64_t, __simd128_int32_t);
__simd128_int64_t __builtin_neon_vqaddsv2di(__simd128_int64_t, __simd128_int64_t);
__simd128_int64_t __builtin_neon_vqadduv2di(__simd128_int64_t, __simd128_int64_t);
__simd128_int64_t __builtin_neon_vqdmlal_lanev2si(__simd128_int64_t, __simd64_int32_t, __simd64_int32_t, int);
__simd128_int64_t __builtin_neon_vqdmlal_nv2si(__simd128_int64_t, __simd64_int32_t, int);
__simd128_int64_t __builtin_neon_vqdmlalv2si(__simd128_int64_t, __simd64_int32_t, __simd64_int32_t);
__simd128_int64_t __builtin_neon_vqdmlsl_lanev2si(__simd128_int64_t, __simd64_int32_t, __simd64_int32_t, int);
__simd128_int64_t __builtin_neon_vqdmlsl_nv2si(__simd128_int64_t, __simd64_int32_t, int);
__simd128_int64_t __builtin_neon_vqdmlslv2si(__simd128_int64_t, __simd64_int32_t, __simd64_int32_t);
__simd128_int64_t __builtin_neon_vqrshlsv2di(__simd128_int64_t, __simd128_int64_t);
__simd128_int64_t __builtin_neon_vqrshluv2di(__simd128_int64_t, __simd128_int64_t);
__simd128_int64_t __builtin_neon_vqshl_s_nv2di(__simd128_int64_t, int);
__simd128_int64_t __builtin_neon_vqshlsv2di(__simd128_int64_t, __simd128_int64_t);
__simd128_int64_t __builtin_neon_vqshl_u_nv2di(__simd128_int64_t, int);
__simd128_int64_t __builtin_neon_vqshlu_nv2di(__simd128_int64_t, int);
__simd128_int64_t __builtin_neon_vqshluv2di(__simd128_int64_t, __simd128_int64_t);
__simd128_int64_t __builtin_neon_vqsubsv2di(__simd128_int64_t, __simd128_int64_t);
__simd128_int64_t __builtin_neon_vqsubuv2di(__simd128_int64_t, __simd128_int64_t);
__simd128_int64_t __builtin_neon_vrshlsv2di(__simd128_int64_t, __simd128_int64_t);
__simd128_int64_t __builtin_neon_vrshluv2di(__simd128_int64_t, __simd128_int64_t);
__simd128_int64_t __builtin_neon_vrshrs_nv2di(__simd128_int64_t, int);
__simd128_int64_t __builtin_neon_vrshru_nv2di(__simd128_int64_t, int);
__simd128_int64_t __builtin_neon_vrsras_nv2di(__simd128_int64_t, __simd128_int64_t, int);
__simd128_int64_t __builtin_neon_vrsrau_nv2di(__simd128_int64_t, __simd128_int64_t, int);
__simd128_int64_t __builtin_neon_vset_lanev2di(long long int, __simd128_int64_t, int);
__simd128_int64_t __builtin_neon_vshl_nv2di(__simd128_int64_t, int);
__simd128_int64_t __builtin_neon_vshlsv2di(__simd128_int64_t, __simd128_int64_t);
__simd128_int64_t __builtin_neon_vshluv2di(__simd128_int64_t, __simd128_int64_t);
__simd128_int64_t __builtin_neon_vshrs_nv2di(__simd128_int64_t, int);
__simd128_int64_t __builtin_neon_vshru_nv2di(__simd128_int64_t, int);
__simd128_int64_t __builtin_neon_vsli_nv2di(__simd128_int64_t, __simd128_int64_t, int);
__simd128_int64_t __builtin_neon_vsras_nv2di(__simd128_int64_t, __simd128_int64_t, int);
__simd128_int64_t __builtin_neon_vsrau_nv2di(__simd128_int64_t, __simd128_int64_t, int);
__simd128_int64_t __builtin_neon_vsri_nv2di(__simd128_int64_t, __simd128_int64_t, int);
__simd128_int64_t __builtin_neon_vsubwsv2si(__simd128_int64_t, __simd64_int32_t);
__simd128_int64_t __builtin_neon_vsubwuv2si(__simd128_int64_t, __simd64_int32_t);
__simd128_int8_t __builtin_neon_vabasv16qi(__simd128_int8_t, __simd128_int8_t, __simd128_int8_t);
__simd128_int8_t __builtin_neon_vabauv16qi(__simd128_int8_t, __simd128_int8_t, __simd128_int8_t);
__simd128_int8_t __builtin_neon_vabdsv16qi(__simd128_int8_t, __simd128_int8_t);
__simd128_int8_t __builtin_neon_vabduv16qi(__simd128_int8_t, __simd128_int8_t);
__simd128_int8_t __builtin_neon_vabsv16qi(__simd128_int8_t);
__simd128_int8_t __builtin_neon_vbslv16qi(__simd128_int8_t, __simd128_int8_t, __simd128_int8_t);
__simd128_int8_t __builtin_neon_vceqv16qi(__simd128_int8_t, __simd128_int8_t);
__simd128_int8_t __builtin_neon_vcgeuv16qi(__simd128_int8_t, __simd128_int8_t);
__simd128_int8_t __builtin_neon_vcgev16qi(__simd128_int8_t, __simd128_int8_t);
__simd128_int8_t __builtin_neon_vcgtuv16qi(__simd128_int8_t, __simd128_int8_t);
__simd128_int8_t __builtin_neon_vcgtv16qi(__simd128_int8_t, __simd128_int8_t);
__simd128_int8_t __builtin_neon_vclsv16qi(__simd128_int8_t);
__simd128_int8_t __builtin_neon_vclzv16qi(__simd128_int8_t);
__simd128_int8_t __builtin_neon_vcntv16qi(__simd128_int8_t);
__simd128_int8_t __builtin_neon_vcombinev8qi(__simd64_int8_t, __simd64_int8_t);
__simd128_int8_t __builtin_neon_vdup_lanev16qi(__simd64_int8_t, int);
__simd128_int8_t __builtin_neon_vextv16qi(__simd128_int8_t, __simd128_int8_t, int);
__simd128_int8_t __builtin_neon_vhaddsv16qi(__simd128_int8_t, __simd128_int8_t);
__simd128_int8_t __builtin_neon_vhadduv16qi(__simd128_int8_t, __simd128_int8_t);
__simd128_int8_t __builtin_neon_vhsubsv16qi(__simd128_int8_t, __simd128_int8_t);
__simd128_int8_t __builtin_neon_vhsubuv16qi(__simd128_int8_t, __simd128_int8_t);
__simd128_int8_t __builtin_neon_vld1_lanev16qi(const signed char*, __simd128_int8_t, int);
__simd128_int8_t __builtin_neon_vmaxsv16qi(__simd128_int8_t, __simd128_int8_t);
__simd128_int8_t __builtin_neon_vmaxuv16qi(__simd128_int8_t, __simd128_int8_t);
__simd128_int8_t __builtin_neon_vminsv16qi(__simd128_int8_t, __simd128_int8_t);
__simd128_int8_t __builtin_neon_vminuv16qi(__simd128_int8_t, __simd128_int8_t);
__simd128_int8_t __builtin_neon_vmlav16qi(__simd128_int8_t, __simd128_int8_t, __simd128_int8_t);
__simd128_int8_t __builtin_neon_vmlsv16qi(__simd128_int8_t, __simd128_int8_t, __simd128_int8_t);
__simd128_int8_t __builtin_neon_vmulpv16qi(__simd128_int8_t, __simd128_int8_t);
__simd128_int8_t __builtin_neon_vmvnv16qi(__simd128_int8_t);
__simd128_int8_t __builtin_neon_vnegv16qi(__simd128_int8_t);
__simd128_int8_t __builtin_neon_vqabsv16qi(__simd128_int8_t);
__simd128_int8_t __builtin_neon_vqaddsv16qi(__simd128_int8_t, __simd128_int8_t);
__simd128_int8_t __builtin_neon_vqadduv16qi(__simd128_int8_t, __simd128_int8_t);
__simd128_int8_t __builtin_neon_vqnegv16qi(__simd128_int8_t);
__simd128_int8_t __builtin_neon_vqrshlsv16qi(__simd128_int8_t, __simd128_int8_t);
__simd128_int8_t __builtin_neon_vqrshluv16qi(__simd128_int8_t, __simd128_int8_t);
__simd128_int8_t __builtin_neon_vqshl_s_nv16qi(__simd128_int8_t, int);
__simd128_int8_t __builtin_neon_vqshlsv16qi(__simd128_int8_t, __simd128_int8_t);
__simd128_int8_t __builtin_neon_vqshl_u_nv16qi(__simd128_int8_t, int);
__simd128_int8_t __builtin_neon_vqshlu_nv16qi(__simd128_int8_t, int);
__simd128_int8_t __builtin_neon_vqshluv16qi(__simd128_int8_t, __simd128_int8_t);
__simd128_int8_t __builtin_neon_vqsubsv16qi(__simd128_int8_t, __simd128_int8_t);
__simd128_int8_t __builtin_neon_vqsubuv16qi(__simd128_int8_t, __simd128_int8_t);
__simd128_int8_t __builtin_neon_vrhaddsv16qi(__simd128_int8_t, __simd128_int8_t);
__simd128_int8_t __builtin_neon_vrhadduv16qi(__simd128_int8_t, __simd128_int8_t);
__simd128_int8_t __builtin_neon_vrshlsv16qi(__simd128_int8_t, __simd128_int8_t);
__simd128_int8_t __builtin_neon_vrshluv16qi(__simd128_int8_t, __simd128_int8_t);
__simd128_int8_t __builtin_neon_vrshrs_nv16qi(__simd128_int8_t, int);
__simd128_int8_t __builtin_neon_vrshru_nv16qi(__simd128_int8_t, int);
__simd128_int8_t __builtin_neon_vrsras_nv16qi(__simd128_int8_t, __simd128_int8_t, int);
__simd128_int8_t __builtin_neon_vrsrau_nv16qi(__simd128_int8_t, __simd128_int8_t, int);
__simd128_int8_t __builtin_neon_vset_lanev16qi(signed char, __simd128_int8_t, int);
__simd128_int8_t __builtin_neon_vshl_nv16qi(__simd128_int8_t, int);
__simd128_int8_t __builtin_neon_vshlsv16qi(__simd128_int8_t, __simd128_int8_t);
__simd128_int8_t __builtin_neon_vshluv16qi(__simd128_int8_t, __simd128_int8_t);
__simd128_int8_t __builtin_neon_vshrs_nv16qi(__simd128_int8_t, int);
__simd128_int8_t __builtin_neon_vshru_nv16qi(__simd128_int8_t, int);
__simd128_int8_t __builtin_neon_vsli_nv16qi(__simd128_int8_t, __simd128_int8_t, int);
__simd128_int8_t __builtin_neon_vsras_nv16qi(__simd128_int8_t, __simd128_int8_t, int);
__simd128_int8_t __builtin_neon_vsrau_nv16qi(__simd128_int8_t, __simd128_int8_t, int);
__simd128_int8_t __builtin_neon_vsri_nv16qi(__simd128_int8_t, __simd128_int8_t, int);
__simd128_int8_t __builtin_neon_vtstv16qi(__simd128_int8_t, __simd128_int8_t);
__simd128_uint32_t __builtin_neon_udot_lanev16qi_uuuus(__simd128_uint32_t, __simd128_uint8_t, __simd64_uint8_t, int);
__simd128_uint32_t __builtin_neon_udotv16qi_uuuu(__simd128_uint32_t, __simd128_uint8_t, __simd128_uint8_t);
__simd64_float16_t __builtin_neon_vabdv4hf(__simd64_float16_t, __simd64_float16_t);
__simd64_float16_t __builtin_neon_vabsv4hf(__simd64_float16_t);
__simd64_float16_t __builtin_neon_vaddv4hf(__simd64_float16_t, __simd64_float16_t);
__simd64_float16_t __builtin_neon_vbslv4hf(__simd64_int16_t, __simd64_float16_t, __simd64_float16_t);
__simd64_float16_t __builtin_neon_vcadd90v4hf(__simd64_float16_t, __simd64_float16_t);
__simd64_float16_t __builtin_neon_vcmla0v4hf(__simd64_float16_t, __simd64_float16_t, __simd64_float16_t);
__simd64_float16_t __builtin_neon_vcmla180v4hf(__simd64_float16_t, __simd64_float16_t, __simd64_float16_t);
__simd64_float16_t __builtin_neon_vcmla270v4hf(__simd64_float16_t, __simd64_float16_t, __simd64_float16_t);
__simd64_float16_t __builtin_neon_vcmla90v4hf(__simd64_float16_t, __simd64_float16_t, __simd64_float16_t);
__simd64_float16_t __builtin_neon_vcmla_lane0v4hf(__simd64_float16_t, __simd64_float16_t, __simd64_float16_t, int);
__simd64_float16_t __builtin_neon_vcmla_lane180v4hf(__simd64_float16_t, __simd64_float16_t, __simd64_float16_t, int);
__simd64_float16_t __builtin_neon_vcmla_lane270v4hf(__simd64_float16_t, __simd64_float16_t, __simd64_float16_t, int);
__simd64_float16_t __builtin_neon_vcmla_lane90v4hf(__simd64_float16_t, __simd64_float16_t, __simd64_float16_t, int);
__simd64_float16_t __builtin_neon_vcmla_laneq0v4hf(__simd64_float16_t, __simd64_float16_t, __simd128_float16_t, int);
__simd64_float16_t __builtin_neon_vcmla_laneq180v4hf(__simd64_float16_t, __simd64_float16_t, __simd128_float16_t, int);
__simd64_float16_t __builtin_neon_vcmla_laneq270v4hf(__simd64_float16_t, __simd64_float16_t, __simd128_float16_t, int);
__simd64_float16_t __builtin_neon_vcmla_laneq90v4hf(__simd64_float16_t, __simd64_float16_t, __simd128_float16_t, int);
__simd64_float16_t __builtin_neon_vcvts_nv4hi(__simd64_int16_t, int);
__simd64_float16_t __builtin_neon_vcvtsv4hi(__simd64_int16_t);
__simd64_float16_t __builtin_neon_vcvtu_nv4hi(__simd64_int16_t, int);
__simd64_float16_t __builtin_neon_vcvtuv4hi(__simd64_int16_t);
__simd64_float16_t __builtin_neon_vdup_lanev4hf(__simd64_float16_t, int);
__simd64_float16_t __builtin_neon_vdup_nv4hf(cov_fp16_t);
__simd64_float16_t __builtin_neon_vextv4hf(__simd64_float16_t, __simd64_float16_t, int);
__simd64_float16_t __builtin_neon_vfmav4hf(__simd64_float16_t, __simd64_float16_t, __simd64_float16_t);
__simd64_float16_t __builtin_neon_vfmsv4hf(__simd64_float16_t, __simd64_float16_t, __simd64_float16_t);
__simd64_float16_t __builtin_neon_vget_highv8hf(__simd128_float16_t);
__simd64_float16_t __builtin_neon_vget_lowv8hf(__simd128_float16_t);
__simd64_float16_t __builtin_neon_vld1v4hf(const cov_fp16_t*);
__simd64_float16_t __builtin_neon_vmaxfv4hf(__simd64_float16_t, __simd64_float16_t);
__simd64_float16_t __builtin_neon_vmaxnmv4hf(__simd64_float16_t, __simd64_float16_t);
__simd64_float16_t __builtin_neon_vminfv4hf(__simd64_float16_t, __simd64_float16_t);
__simd64_float16_t __builtin_neon_vminnmv4hf(__simd64_float16_t, __simd64_float16_t);
__simd64_float16_t __builtin_neon_vmulfv4hf(__simd64_float16_t, __simd64_float16_t);
__simd64_float16_t __builtin_neon_vmul_lanev4hf(__simd64_float16_t, __simd64_float16_t, int);
__simd64_float16_t __builtin_neon_vmul_nv4hf(__simd64_float16_t, cov_fp16_t);
__simd64_float16_t __builtin_neon_vnegv4hf(__simd64_float16_t);
__simd64_float16_t __builtin_neon_vpaddv4hf(__simd64_float16_t, __simd64_float16_t);
__simd64_float16_t __builtin_neon_vpmaxfv4hf(__simd64_float16_t, __simd64_float16_t);
__simd64_float16_t __builtin_neon_vpminfv4hf(__simd64_float16_t, __simd64_float16_t);
__simd64_float16_t __builtin_neon_vrecpev4hf(__simd64_float16_t);
__simd64_float16_t __builtin_neon_vrecpsv4hf(__simd64_float16_t, __simd64_float16_t);
__simd64_float16_t __builtin_neon_vrndav4hf(__simd64_float16_t);
__simd64_float16_t __builtin_neon_vrndmv4hf(__simd64_float16_t);
__simd64_float16_t __builtin_neon_vrndnv4hf(__simd64_float16_t);
__simd64_float16_t __builtin_neon_vrndpv4hf(__simd64_float16_t);
__simd64_float16_t __builtin_neon_vrndv4hf(__simd64_float16_t);
__simd64_float16_t __builtin_neon_vrndxv4hf(__simd64_float16_t);
__simd64_float16_t __builtin_neon_vrsqrtev4hf(__simd64_float16_t);
__simd64_float16_t __builtin_neon_vrsqrtsv4hf(__simd64_float16_t, __simd64_float16_t);
__simd64_float16_t __builtin_neon_vsubv4hf(__simd64_float16_t, __simd64_float16_t);
__simd64_float32_t __builtin_neon_vabdfv2sf (__simd64_float32_t __a, __simd64_float32_t __b);
__simd64_float32_t __builtin_neon_vabsv2sf (__simd64_float32_t __a);
__simd64_float32_t  __builtin_neon_vaddv2sf (__simd64_float32_t __a, __simd64_float32_t __b);
__simd64_float32_t __builtin_neon_vbslv2sf (__simd64_int32_t __a, __simd64_float32_t __b, __simd64_float32_t __c);
__simd64_float32_t __builtin_neon_vcadd90v2sf(__simd64_float32_t, __simd64_float32_t);
__simd64_float32_t __builtin_neon_vcmla0v2sf(__simd64_float32_t, __simd64_float32_t, __simd64_float32_t);
__simd64_float32_t __builtin_neon_vcmla180v2sf(__simd64_float32_t, __simd64_float32_t, __simd64_float32_t);
__simd64_float32_t __builtin_neon_vcmla270v2sf(__simd64_float32_t, __simd64_float32_t, __simd64_float32_t);
__simd64_float32_t __builtin_neon_vcmla90v2sf(__simd64_float32_t, __simd64_float32_t, __simd64_float32_t);
__simd64_float32_t __builtin_neon_vcmla_lane0v2sf(__simd64_float32_t, __simd64_float32_t, __simd64_float32_t, int);
__simd64_float32_t __builtin_neon_vcmla_lane180v2sf(__simd64_float32_t, __simd64_float32_t, __simd64_float32_t, int);
__simd64_float32_t __builtin_neon_vcmla_lane270v2sf(__simd64_float32_t, __simd64_float32_t, __simd64_float32_t, int);
__simd64_float32_t __builtin_neon_vcmla_lane90v2sf(__simd64_float32_t, __simd64_float32_t, __simd64_float32_t, int);
__simd64_float32_t __builtin_neon_vcmla_laneq0v2sf(__simd64_float32_t, __simd64_float32_t, __simd128_float32_t, int);
__simd64_float32_t __builtin_neon_vcmla_laneq180v2sf(__simd64_float32_t, __simd64_float32_t, __simd128_float32_t, int);
__simd64_float32_t __builtin_neon_vcmla_laneq270v2sf(__simd64_float32_t, __simd64_float32_t, __simd128_float32_t, int);
__simd64_float32_t __builtin_neon_vcmla_laneq90v2sf(__simd64_float32_t, __simd64_float32_t, __simd128_float32_t, int);
__simd64_float32_t __builtin_neon_vcreatev2sf (__builtin_neon_di __a);
__simd64_float32_t __builtin_neon_vcvts_nv2si (__simd64_int32_t __a, const int __b);
__simd64_float32_t __builtin_neon_vcvtsv2si (__simd64_int32_t __a);
__simd64_float32_t __builtin_neon_vcvtu_nv2si (__simd64_int32_t __a, const int __b);
__simd64_float32_t __builtin_neon_vcvtuv2si (__simd64_int32_t  __a);
__simd64_float16_t __builtin_neon_vcvtv4hfv4sf(__simd128_float32_t);
__simd64_float32_t __builtin_neon_vdup_lanev2sf (__simd64_float32_t __a, const int __b);
__simd64_float32_t __builtin_neon_vdup_nv2sf (__builtin_neon_sf __a);
__simd64_float32_t __builtin_neon_vextv2sf (__simd64_float32_t __a, __simd64_float32_t __b, const int __c);
__simd64_float32_t __builtin_neon_vfmal_highv2sf(__simd64_float32_t, __simd64_float16_t, __simd64_float16_t);
__simd64_float32_t __builtin_neon_vfmal_lane_highv2sf(__simd64_float32_t, __simd64_float16_t, __simd64_float16_t, int);
__simd64_float32_t __builtin_neon_vfmal_lane_highv8hfv2sf(__simd64_float32_t, __simd64_float16_t, __simd128_float16_t, int);
__simd64_float32_t __builtin_neon_vfmal_lane_lowv2sf(__simd64_float32_t, __simd64_float16_t, __simd64_float16_t, int);
__simd64_float32_t __builtin_neon_vfmal_lane_lowv8hfv2sf(__simd64_float32_t, __simd64_float16_t, __simd128_float16_t, int);
__simd64_float32_t __builtin_neon_vfmal_lowv2sf(__simd64_float32_t, __simd64_float16_t, __simd64_float16_t);
__simd64_float32_t __builtin_neon_vfmav2sf (__simd64_float32_t __a, __simd64_float32_t __b, __simd64_float32_t __c);
__simd64_float32_t __builtin_neon_vfmsl_highv2sf(__simd64_float32_t, __simd64_float16_t, __simd64_float16_t);
__simd64_float32_t __builtin_neon_vfmsl_lane_highv2sf(__simd64_float32_t, __simd64_float16_t, __simd64_float16_t, int);
__simd64_float32_t __builtin_neon_vfmsl_lane_highv8hfv2sf(__simd64_float32_t, __simd64_float16_t, __simd128_float16_t, int);
__simd64_float32_t __builtin_neon_vfmsl_lane_lowv2sf(__simd64_float32_t, __simd64_float16_t, __simd64_float16_t, int);
__simd64_float32_t __builtin_neon_vfmsl_lane_lowv8hfv2sf(__simd64_float32_t, __simd64_float16_t, __simd128_float16_t, int);
__simd64_float32_t __builtin_neon_vfmsl_lowv2sf(__simd64_float32_t, __simd64_float16_t, __simd64_float16_t);
__simd64_float32_t __builtin_neon_vfmsv2sf (__simd64_float32_t __a, __simd64_float32_t __b, __simd64_float32_t __c);
__simd64_float32_t __builtin_neon_vget_highv4sf (__simd128_float32_t __a);
__simd64_float32_t __builtin_neon_vget_lowv4sf (__simd128_float32_t __a);
__simd64_float32_t __builtin_neon_vld1_dupv2sf (const __builtin_neon_sf *  __a);
__simd64_float32_t __builtin_neon_vld1_lanev2sf (const float * __a, __simd64_float32_t __b, const int __c);
__simd64_float32_t __builtin_neon_vld1v2sf (const __builtin_neon_sf *  __a);
__simd64_float32_t __builtin_neon_vmaxfv2sf (__simd64_float32_t __a, __simd64_float32_t __b);
__simd64_float32_t __builtin_neon_vmaxnmv2sf(__simd64_float32_t, __simd64_float32_t);
__simd64_float32_t __builtin_neon_vminfv2sf (__simd64_float32_t __a, __simd64_float32_t __b);
__simd64_float32_t __builtin_neon_vminnmv2sf(__simd64_float32_t, __simd64_float32_t);
__simd64_float32_t __builtin_neon_vmla_lanev2sf (__simd64_float32_t __a, __simd64_float32_t __b, __simd64_float32_t __c, const int __d);
__simd64_float32_t __builtin_neon_vmla_nv2sf (__simd64_float32_t __a, __simd64_float32_t __b, float __c);
__simd64_float32_t __builtin_neon_vmlav2sf (__simd64_float32_t __a, __simd64_float32_t __b, __simd64_float32_t __c);
__simd64_float32_t __builtin_neon_vmls_lanev2sf (__simd64_float32_t __a, __simd64_float32_t __b, __simd64_float32_t __c, const int __d);
__simd64_float32_t __builtin_neon_vmls_nv2sf (__simd64_float32_t __a, __simd64_float32_t __b, float __c);
__simd64_float32_t __builtin_neon_vmlsv2sf (__simd64_float32_t __a, __simd64_float32_t __b, __simd64_float32_t __c);
__simd64_float32_t  __builtin_neon_vmulfv2sf (__simd64_float32_t __a, __simd64_float32_t __b);
__simd64_float32_t __builtin_neon_vmul_lanev2sf (__simd64_float32_t __a, __simd64_float32_t __b, const int __c);
__simd64_float32_t __builtin_neon_vmul_nv2sf (__simd64_float32_t __a, float __b);
__simd64_float32_t __builtin_neon_vnegv2sf (__simd64_float32_t __a);
__simd64_float32_t __builtin_neon_vpaddv2sf (__simd64_float32_t __a, __simd64_float32_t __b);
__simd64_float32_t __builtin_neon_vpmaxfv2sf (__simd64_float32_t __a, __simd64_float32_t __b);
__simd64_float32_t __builtin_neon_vpminfv2sf (__simd64_float32_t __a, __simd64_float32_t __b);
__simd64_float32_t __builtin_neon_vrecpev2sf (__simd64_float32_t __a);
__simd64_float32_t __builtin_neon_vrecpsv2sf (__simd64_float32_t __a, __simd64_float32_t __b);
__simd64_float32_t __builtin_neon_vrintav2sf (__simd64_float32_t __a);
__simd64_float32_t __builtin_neon_vrintmv2sf (__simd64_float32_t __a);
__simd64_float32_t __builtin_neon_vrintnv2sf (__simd64_float32_t __a);
__simd64_float32_t __builtin_neon_vrintpv2sf (__simd64_float32_t __a);
__simd64_float32_t __builtin_neon_vrintxv2sf (__simd64_float32_t __a);
__simd64_float32_t __builtin_neon_vrintzv2sf (__simd64_float32_t __a);
__simd64_float32_t __builtin_neon_vrsqrtev2sf (__simd64_float32_t __a);
__simd64_float32_t __builtin_neon_vrsqrtsv2sf (__simd64_float32_t __a, __simd64_float32_t __b);
__simd64_float32_t __builtin_neon_vset_lanev2sf (float __a, __simd64_float32_t __b, const int __c);
void __builtin_neon_vst1_lanev2sf(float*, __simd64_float32_t, int);
void __builtin_neon_vst1v2sf(float*, __simd64_float32_t);
__simd64_float32_t  __builtin_neon_vsubv2sf (__simd64_float32_t __a, __simd64_float32_t __b);
__simd64_int16_t __builtin_neon_vabasv4hi(__simd64_int16_t, __simd64_int16_t, __simd64_int16_t);
__simd64_int16_t __builtin_neon_vabauv4hi(__simd64_int16_t, __simd64_int16_t, __simd64_int16_t);
__simd64_int16_t __builtin_neon_vabdsv4hi(__simd64_int16_t, __simd64_int16_t);
__simd64_int16_t __builtin_neon_vabduv4hi(__simd64_int16_t, __simd64_int16_t);
__simd64_int16_t __builtin_neon_vabsv4hi(__simd64_int16_t);
__simd64_int16_t __builtin_neon_vbslv4hi(__simd64_int16_t, __simd64_int16_t, __simd64_int16_t);
__simd64_int16_t __builtin_neon_vcagev4hf(__simd64_float16_t, __simd64_float16_t);
__simd64_int16_t __builtin_neon_vcagtv4hf(__simd64_float16_t, __simd64_float16_t);
__simd64_int16_t __builtin_neon_vcalev4hf(__simd64_float16_t, __simd64_float16_t);
__simd64_int16_t __builtin_neon_vcaltv4hf(__simd64_float16_t, __simd64_float16_t);
__simd64_int16_t __builtin_neon_vceqv4hf(__simd64_float16_t, __simd64_float16_t);
__simd64_int16_t __builtin_neon_vceqv4hi(__simd64_int16_t, __simd64_int16_t);
__simd64_int16_t __builtin_neon_vceqzv4hf(__simd64_float16_t);
__simd64_int16_t __builtin_neon_vcgeuv4hi(__simd64_int16_t, __simd64_int16_t);
__simd64_int16_t __builtin_neon_vcgev4hf(__simd64_float16_t, __simd64_float16_t);
__simd64_int16_t __builtin_neon_vcgev4hi(__simd64_int16_t, __simd64_int16_t);
__simd64_int16_t __builtin_neon_vcgezv4hf(__simd64_float16_t);
__simd64_int16_t __builtin_neon_vcgtuv4hi(__simd64_int16_t, __simd64_int16_t);
__simd64_int16_t __builtin_neon_vcgtv4hf(__simd64_float16_t, __simd64_float16_t);
__simd64_int16_t __builtin_neon_vcgtv4hi(__simd64_int16_t, __simd64_int16_t);
__simd64_int16_t __builtin_neon_vcgtzv4hf(__simd64_float16_t);
__simd64_int16_t __builtin_neon_vclev4hf(__simd64_float16_t, __simd64_float16_t);
__simd64_int16_t __builtin_neon_vclezv4hf(__simd64_float16_t);
__simd64_int16_t __builtin_neon_vclsv4hi(__simd64_int16_t);
__simd64_int16_t __builtin_neon_vcltv4hf(__simd64_float16_t, __simd64_float16_t);
__simd64_int16_t __builtin_neon_vcltzv4hf(__simd64_float16_t);
__simd64_int16_t __builtin_neon_vclzv4hi(__simd64_int16_t);
__simd64_int16_t __builtin_neon_vcvtasv4hf(__simd64_float16_t);
__simd64_int16_t __builtin_neon_vcvtauv4hf(__simd64_float16_t);
__simd64_int16_t __builtin_neon_vcvtmsv4hf(__simd64_float16_t);
__simd64_int16_t __builtin_neon_vcvtmuv4hf(__simd64_float16_t);
__simd64_int16_t __builtin_neon_vcvtnsv4hf(__simd64_float16_t);
__simd64_int16_t __builtin_neon_vcvtnuv4hf(__simd64_float16_t);
__simd64_int16_t __builtin_neon_vcvtpsv4hf(__simd64_float16_t);
__simd64_int16_t __builtin_neon_vcvtpuv4hf(__simd64_float16_t);
__simd64_int16_t __builtin_neon_vcvts_nv4hf(__simd64_float16_t, int);
__simd64_int16_t __builtin_neon_vcvtsv4hf(__simd64_float16_t);
__simd64_int16_t __builtin_neon_vcvtu_nv4hf(__simd64_float16_t, int);
__simd64_int16_t __builtin_neon_vcvtuv4hf(__simd64_float16_t);
__simd64_int16_t __builtin_neon_vdup_lanev4hi(__simd64_int16_t, int);
__simd64_int16_t __builtin_neon_vextv4hi(__simd64_int16_t, __simd64_int16_t, int);
__simd64_int16_t __builtin_neon_vget_highv8hi(__simd128_int16_t);
__simd64_int16_t __builtin_neon_vget_lowv8hi(__simd128_int16_t);
__simd64_int16_t __builtin_neon_vhaddsv4hi(__simd64_int16_t, __simd64_int16_t);
__simd64_int16_t __builtin_neon_vhadduv4hi(__simd64_int16_t, __simd64_int16_t);
__simd64_int16_t __builtin_neon_vhsubsv4hi(__simd64_int16_t, __simd64_int16_t);
__simd64_int16_t __builtin_neon_vhsubuv4hi(__simd64_int16_t, __simd64_int16_t);
__simd64_int16_t __builtin_neon_vld1_lanev4hi(const short int*, __simd64_int16_t, int);
__simd64_int16_t __builtin_neon_vmaxsv4hi(__simd64_int16_t, __simd64_int16_t);
__simd64_int16_t __builtin_neon_vmaxuv4hi(__simd64_int16_t, __simd64_int16_t);
__simd64_int16_t __builtin_neon_vminsv4hi(__simd64_int16_t, __simd64_int16_t);
__simd64_int16_t __builtin_neon_vminuv4hi(__simd64_int16_t, __simd64_int16_t);
__simd64_int16_t __builtin_neon_vmla_lanev4hi(__simd64_int16_t, __simd64_int16_t, __simd64_int16_t, int);
__simd64_int16_t __builtin_neon_vmla_nv4hi(__simd64_int16_t, __simd64_int16_t, short int);
__simd64_int16_t __builtin_neon_vmlav4hi(__simd64_int16_t, __simd64_int16_t, __simd64_int16_t);
__simd64_int16_t __builtin_neon_vmls_lanev4hi(__simd64_int16_t, __simd64_int16_t, __simd64_int16_t, int);
__simd64_int16_t __builtin_neon_vmls_nv4hi(__simd64_int16_t, __simd64_int16_t, short int);
__simd64_int16_t __builtin_neon_vmlsv4hi(__simd64_int16_t, __simd64_int16_t, __simd64_int16_t);
__simd64_int16_t __builtin_neon_vmul_lanev4hi(__simd64_int16_t, __simd64_int16_t, int);
__simd64_int16_t __builtin_neon_vmul_nv4hi(__simd64_int16_t, short int);
__simd64_int16_t __builtin_neon_vmvnv4hi(__simd64_int16_t);
__simd64_int16_t __builtin_neon_vnegv4hi(__simd64_int16_t);
__simd64_int16_t __builtin_neon_vpadalsv8qi(__simd64_int16_t, __simd64_int8_t);
__simd64_int16_t __builtin_neon_vpadaluv8qi(__simd64_int16_t, __simd64_int8_t);
__simd64_int16_t __builtin_neon_vpaddlsv8qi(__simd64_int8_t);
__simd64_int16_t __builtin_neon_vpaddluv8qi(__simd64_int8_t);
__simd64_int16_t __builtin_neon_vpaddv4hi(__simd64_int16_t, __simd64_int16_t);
__simd64_int16_t __builtin_neon_vpmaxsv4hi(__simd64_int16_t, __simd64_int16_t);
__simd64_int16_t __builtin_neon_vpmaxuv4hi(__simd64_int16_t, __simd64_int16_t);
__simd64_int16_t __builtin_neon_vpminsv4hi(__simd64_int16_t, __simd64_int16_t);
__simd64_int16_t __builtin_neon_vpminuv4hi(__simd64_int16_t, __simd64_int16_t);
__simd64_int16_t __builtin_neon_vqabsv4hi(__simd64_int16_t);
__simd64_int16_t __builtin_neon_vqaddsv4hi(__simd64_int16_t, __simd64_int16_t);
__simd64_int16_t __builtin_neon_vqadduv4hi(__simd64_int16_t, __simd64_int16_t);
__simd64_int16_t __builtin_neon_vqdmulh_lanev4hi(__simd64_int16_t, __simd64_int16_t, int);
__simd64_int16_t __builtin_neon_vqdmulh_nv4hi(__simd64_int16_t, short int);
__simd64_int16_t __builtin_neon_vqdmulhv4hi(__simd64_int16_t, __simd64_int16_t);
__simd64_int16_t __builtin_neon_vqnegv4hi(__simd64_int16_t);
__simd64_int16_t __builtin_neon_vqrdmlah_lanev4hi(__simd64_int16_t, __simd64_int16_t, __simd64_int16_t, int);
__simd64_int16_t __builtin_neon_vqrdmlahv4hi(__simd64_int16_t, __simd64_int16_t, __simd64_int16_t);
__simd64_int16_t __builtin_neon_vqrdmlsh_lanev4hi(__simd64_int16_t, __simd64_int16_t, __simd64_int16_t, int);
__simd64_int16_t __builtin_neon_vqrdmlshv4hi(__simd64_int16_t, __simd64_int16_t, __simd64_int16_t);
__simd64_int16_t __builtin_neon_vqrdmulh_lanev4hi(__simd64_int16_t, __simd64_int16_t, int);
__simd64_int16_t __builtin_neon_vqrdmulh_nv4hi(__simd64_int16_t, short int);
__simd64_int16_t __builtin_neon_vqrdmulhv4hi(__simd64_int16_t, __simd64_int16_t);
__simd64_int16_t __builtin_neon_vqrshlsv4hi(__simd64_int16_t, __simd64_int16_t);
__simd64_int16_t __builtin_neon_vqrshluv4hi(__simd64_int16_t, __simd64_int16_t);
__simd64_int16_t __builtin_neon_vqshl_s_nv4hi(__simd64_int16_t, int);
__simd64_int16_t __builtin_neon_vqshlsv4hi(__simd64_int16_t, __simd64_int16_t);
__simd64_int16_t __builtin_neon_vqshl_u_nv4hi(__simd64_int16_t, int);
__simd64_int16_t __builtin_neon_vqshlu_nv4hi(__simd64_int16_t, int);
__simd64_int16_t __builtin_neon_vqshluv4hi(__simd64_int16_t, __simd64_int16_t);
__simd64_int16_t __builtin_neon_vqsubsv4hi(__simd64_int16_t, __simd64_int16_t);
__simd64_int16_t __builtin_neon_vqsubuv4hi(__simd64_int16_t, __simd64_int16_t);
__simd64_int16_t __builtin_neon_vrhaddsv4hi(__simd64_int16_t, __simd64_int16_t);
__simd64_int16_t __builtin_neon_vrhadduv4hi(__simd64_int16_t, __simd64_int16_t);
__simd64_int16_t __builtin_neon_vrshlsv4hi(__simd64_int16_t, __simd64_int16_t);
__simd64_int16_t __builtin_neon_vrshluv4hi(__simd64_int16_t, __simd64_int16_t);
__simd64_int16_t __builtin_neon_vrshrs_nv4hi(__simd64_int16_t, int);
__simd64_int16_t __builtin_neon_vrshru_nv4hi(__simd64_int16_t, int);
__simd64_int16_t __builtin_neon_vrsras_nv4hi(__simd64_int16_t, __simd64_int16_t, int);
__simd64_int16_t __builtin_neon_vrsrau_nv4hi(__simd64_int16_t, __simd64_int16_t, int);
__simd64_int16_t __builtin_neon_vset_lanev4hi(short int, __simd64_int16_t, int);
__simd64_int16_t __builtin_neon_vshl_nv4hi(__simd64_int16_t, int);
__simd64_int16_t __builtin_neon_vshlsv4hi(__simd64_int16_t, __simd64_int16_t);
__simd64_int16_t __builtin_neon_vshluv4hi(__simd64_int16_t, __simd64_int16_t);
__simd64_int16_t __builtin_neon_vshrs_nv4hi(__simd64_int16_t, int);
__simd64_int16_t __builtin_neon_vshru_nv4hi(__simd64_int16_t, int);
__simd64_int16_t __builtin_neon_vsli_nv4hi(__simd64_int16_t, __simd64_int16_t, int);
__simd64_int16_t __builtin_neon_vsras_nv4hi(__simd64_int16_t, __simd64_int16_t, int);
__simd64_int16_t __builtin_neon_vsrau_nv4hi(__simd64_int16_t, __simd64_int16_t, int);
__simd64_int16_t __builtin_neon_vsri_nv4hi(__simd64_int16_t, __simd64_int16_t, int);
__simd64_int16_t __builtin_neon_vtstv4hi(__simd64_int16_t, __simd64_int16_t);
__simd64_int32_t __builtin_neon_sdot_lanev8qi(__simd64_int32_t, __simd64_int8_t, __simd64_int8_t, int);
__simd64_int32_t __builtin_neon_sdotv8qi(__simd64_int32_t, __simd64_int8_t, __simd64_int8_t);
__simd64_int32_t __builtin_neon_vabasv2si (__simd64_int32_t __a, __simd64_int32_t __b, __simd64_int32_t __c);
__simd64_int32_t __builtin_neon_vabauv2si (__simd64_int32_t  __a, __simd64_int32_t  __b, __simd64_int32_t  __c);
__simd64_int32_t __builtin_neon_vabdsv2si (__simd64_int32_t __a, __simd64_int32_t __b);
__simd64_int32_t __builtin_neon_vabduv2si (__simd64_int32_t  __a, __simd64_int32_t  __b);
__simd64_int32_t __builtin_neon_vabsv2si (__simd64_int32_t __a);
__simd64_int32_t __builtin_neon_vaddhnv2di(__simd128_int64_t, __simd128_int64_t);
__simd64_int32_t __builtin_neon_vaddhnv4si (__simd128_int32_t __a, __simd128_int32_t __b);
__simd64_int32_t __builtin_neon_vbslv2si (__simd64_int32_t __a, __simd64_int32_t __b, __simd64_int32_t __c);
__simd64_int32_t __builtin_neon_vcagev2sf (__simd64_float32_t __a, __simd64_float32_t __b);
__simd64_int32_t __builtin_neon_vcagtv2sf (__simd64_float32_t __a, __simd64_float32_t __b);
__simd64_int32_t __builtin_neon_vceqv2sf (__simd64_float32_t __a, __simd64_float32_t __b);
__simd64_int32_t __builtin_neon_vceqv2si (__simd64_int32_t __a, __simd64_int32_t __b);
__simd64_int32_t __builtin_neon_vcgeuv2si (__simd64_int32_t __a, __simd64_int32_t __b);
__simd64_int32_t __builtin_neon_vcgev2sf (__simd64_float32_t __a, __simd64_float32_t __b);
__simd64_int32_t __builtin_neon_vcgev2si (__simd64_int32_t __a, __simd64_int32_t __b);
__simd64_int32_t __builtin_neon_vcgtuv2si (__simd64_int32_t __a, __simd64_int32_t __b);
__simd64_int32_t __builtin_neon_vcgtv2sf (__simd64_float32_t __a, __simd64_float32_t __b);
__simd64_int32_t __builtin_neon_vcgtv2si (__simd64_int32_t __a, __simd64_int32_t __b);
__simd64_int32_t __builtin_neon_vclsv2si (__simd64_int32_t __a);
__simd64_int32_t __builtin_neon_vclzv2si (__simd64_int32_t __a);
__simd64_int32_t __builtin_neon_vcreatev2si (__builtin_neon_di __a);
__simd64_int16_t __builtin_neon_vcreatev4hi(long long int);
__simd64_int8_t __builtin_neon_vcreatev8qi(long long int);
__simd64_int32_t __builtin_neon_vcvts_nv2sf (__simd64_float32_t __a, const int __b);
__simd64_int32_t __builtin_neon_vcvtsv2sf (__simd64_float32_t __a);
__simd64_int32_t __builtin_neon_vcvtu_nv2sf (__simd64_float32_t __a, const int __b);
__simd64_int32_t __builtin_neon_vcvtuv2sf (__simd64_float32_t __a);
__simd64_int32_t __builtin_neon_vdup_lanev2si (__simd64_int32_t __a, const int __b);
__simd64_int32_t __builtin_neon_vdup_nv2si (__builtin_neon_si __a);
__simd64_int16_t __builtin_neon_vdup_nv4hi (__builtin_neon_hi __a);
__simd64_int8_t __builtin_neon_vdup_nv8qi(signed char);
__simd64_int32_t __builtin_neon_vextv2si (__simd64_int32_t __a, __simd64_int32_t __b, const int __c);
__simd64_int32_t __builtin_neon_vget_highv4si (__simd128_int32_t __a);
__simd64_int32_t __builtin_neon_vget_lowv4si (__simd128_int32_t __a);
__simd64_int32_t __builtin_neon_vhaddsv2si (__simd64_int32_t __a, __simd64_int32_t __b);
__simd64_int32_t __builtin_neon_vhadduv2si (__simd64_int32_t  __a, __simd64_int32_t  __b);
__simd64_int32_t __builtin_neon_vhsubsv2si (__simd64_int32_t __a, __simd64_int32_t __b);
__simd64_int32_t __builtin_neon_vhsubuv2si (__simd64_int32_t  __a, __simd64_int32_t  __b);
__simd64_int32_t __builtin_neon_vld1_dupv2si (const __builtin_neon_si *  __a);
__simd64_int16_t __builtin_neon_vld1_dupv4hi(const short int*);
__simd64_int8_t __builtin_neon_vld1_dupv8qi(const signed char*);
__simd64_int32_t __builtin_neon_vld1_lanev2si (const int * __a, __simd64_int32_t __b, const int __c);
__simd64_int32_t __builtin_neon_vld1v2si (const __builtin_neon_si *  __a);
__simd64_int16_t __builtin_neon_vld1v4hi(const short int*);
__simd64_int8_t __builtin_neon_vld1v8qi(const signed char*);
__simd64_int32_t __builtin_neon_vmaxsv2si (__simd64_int32_t __a, __simd64_int32_t __b);
__simd64_int32_t __builtin_neon_vmaxuv2si (__simd64_int32_t  __a, __simd64_int32_t  __b);
__simd64_int32_t __builtin_neon_vminsv2si (__simd64_int32_t __a, __simd64_int32_t __b);
__simd64_int32_t __builtin_neon_vminuv2si (__simd64_int32_t  __a, __simd64_int32_t  __b);
__simd64_int32_t __builtin_neon_vmla_lanev2si (__simd64_int32_t __a, __simd64_int32_t __b, __simd64_int32_t __c, const int __d);
__simd64_int32_t __builtin_neon_vmla_nv2si (__simd64_int32_t __a, __simd64_int32_t __a, int __c);
__simd64_int32_t __builtin_neon_vmlav2si (__simd64_int32_t __a, __simd64_int32_t __b, __simd64_int32_t __c);
__simd64_int32_t __builtin_neon_vmls_lanev2si (__simd64_int32_t __a, __simd64_int32_t __b, __simd64_int32_t __c, const int __d);
__simd64_int32_t __builtin_neon_vmls_nv2si (__simd64_int32_t __a, __simd64_int32_t __a, int __c);
__simd64_int32_t __builtin_neon_vmlsv2si (__simd64_int32_t __a, __simd64_int32_t __b, __simd64_int32_t __c);
__simd64_int32_t __builtin_neon_vmovnv2di(__simd128_int64_t);
__simd64_int32_t __builtin_neon_vmovnv4si (__simd128_int32_t __a);
__simd64_int32_t __builtin_neon_vmul_lanev2si (__simd64_int32_t __a, __simd64_int32_t __b, const int __c);
__simd64_int32_t __builtin_neon_vmul_nv2si (__simd64_int32_t __a, int __b);
__simd64_int32_t __builtin_neon_vmvnv2si (__simd64_int32_t __a);
__simd64_int32_t __builtin_neon_vnegv2si (__simd64_int32_t __a);
__simd64_int32_t __builtin_neon_vpadalsv4hi(__simd64_int32_t, __simd64_int16_t);
__simd64_int32_t __builtin_neon_vpadaluv4hi(__simd64_int32_t, __simd64_int16_t);
__simd64_int32_t __builtin_neon_vpaddlsv4hi(__simd64_int16_t);
__simd64_int32_t __builtin_neon_vpaddluv4hi(__simd64_int16_t);
__simd64_int32_t __builtin_neon_vpaddv2si (__simd64_int32_t __a, __simd64_int32_t __b);
__simd64_int32_t __builtin_neon_vpmaxsv2si (__simd64_int32_t __a, __simd64_int32_t __b);
__simd64_int32_t __builtin_neon_vpmaxuv2si (__simd64_int32_t  __a, __simd64_int32_t  __b);
__simd64_int32_t __builtin_neon_vpminsv2si (__simd64_int32_t __a, __simd64_int32_t __b);
__simd64_int32_t __builtin_neon_vpminuv2si (__simd64_int32_t  __a, __simd64_int32_t  __b);
__simd64_int32_t __builtin_neon_vqabsv2si (__simd64_int32_t __a);
__simd64_int32_t __builtin_neon_vqaddsv2si (__simd64_int32_t __a, __simd64_int32_t __b);
__simd64_int32_t __builtin_neon_vqadduv2si (__simd64_int32_t  __a, __simd64_int32_t  __b);
__simd64_int32_t __builtin_neon_vqdmulh_lanev2si (__simd64_int32_t __a, __simd64_int32_t __b, const int __c);
__simd64_int32_t __builtin_neon_vqdmulh_nv2si (__simd64_int32_t __a, int __b);
__simd64_int32_t __builtin_neon_vqdmulhv2si (__simd64_int32_t __a, __simd64_int32_t __b);
__simd64_int32_t __builtin_neon_vqmovnsv2di(__simd128_int64_t);
__simd64_int32_t __builtin_neon_vqmovnsv4si (__simd128_int32_t __a);
__simd64_int32_t __builtin_neon_vqmovnuv2di(__simd128_int64_t);
__simd64_int32_t __builtin_neon_vqmovnuv4si (__simd128_int32_t  __a);
__simd64_int32_t __builtin_neon_vqmovunv2di(__simd128_int64_t);
__simd64_int32_t __builtin_neon_vqmovunv4si (__simd128_int32_t __a);
__simd64_int32_t __builtin_neon_vqnegv2si (__simd64_int32_t __a);
__simd64_int32_t __builtin_neon_vqrdmlah_lanev2si(__simd64_int32_t, __simd64_int32_t, __simd64_int32_t, int);
__simd64_int32_t __builtin_neon_vqrdmlahv2si(__simd64_int32_t, __simd64_int32_t, __simd64_int32_t);
__simd64_int32_t __builtin_neon_vqrdmlsh_lanev2si(__simd64_int32_t, __simd64_int32_t, __simd64_int32_t, int);
__simd64_int32_t __builtin_neon_vqrdmlshv2si(__simd64_int32_t, __simd64_int32_t, __simd64_int32_t);
__simd64_int32_t __builtin_neon_vqrdmulh_lanev2si (__simd64_int32_t __a, __simd64_int32_t __b, const int __c);
__simd64_int32_t __builtin_neon_vqrdmulh_nv2si (__simd64_int32_t __a, int __b);
__simd64_int32_t __builtin_neon_vqrdmulhv2si (__simd64_int32_t __a, __simd64_int32_t __b);
__simd64_int32_t __builtin_neon_vqrshlsv2si (__simd64_int32_t __a, __simd64_int32_t __b);
__simd64_int32_t __builtin_neon_vqrshluv2si (__simd64_int32_t __a, __simd64_int32_t __b);
__simd64_int32_t __builtin_neon_vqrshrns_nv2di(__simd128_int64_t, int);
__simd64_int32_t __builtin_neon_vqrshrns_nv4si (__simd128_int32_t __a, const int __b);
__simd64_int32_t __builtin_neon_vqrshrnu_nv2di(__simd128_int64_t, int);
__simd64_int32_t __builtin_neon_vqrshrnu_nv4si (__simd128_int32_t  __a, const int __b);
__simd64_int32_t __builtin_neon_vqrshrun_nv2di(__simd128_int64_t, int);
__simd64_int32_t __builtin_neon_vqrshrun_nv4si (__simd128_int32_t __a, const int __b);
__simd64_int32_t __builtin_neon_vqshl_s_nv2si (__simd64_int32_t __a, const int __b);
__simd64_int32_t __builtin_neon_vqshlsv2si (__simd64_int32_t __a, __simd64_int32_t __b);
__simd64_int32_t __builtin_neon_vqshl_u_nv2si (__simd64_int32_t __a, const int __b);
__simd64_int32_t __builtin_neon_vqshlu_nv2si (__simd64_int32_t __a, const int __b);
__simd64_int32_t __builtin_neon_vqshluv2si (__simd64_int32_t __a, __simd64_int32_t __b);
__simd64_int32_t __builtin_neon_vqshrns_nv2di(__simd128_int64_t, int);
__simd64_int32_t __builtin_neon_vqshrns_nv4si (__simd128_int32_t __a, const int __b);
__simd64_int32_t __builtin_neon_vqshrnu_nv2di(__simd128_int64_t, int);
__simd64_int32_t __builtin_neon_vqshrnu_nv4si (__simd128_int32_t  __a, const int __b);
__simd64_int32_t __builtin_neon_vqshrun_nv2di(__simd128_int64_t, int);
__simd64_int32_t __builtin_neon_vqshrun_nv4si (__simd128_int32_t __a, const int __b);
__simd64_int32_t __builtin_neon_vqsubsv2si (__simd64_int32_t __a, __simd64_int32_t __b);
__simd64_int32_t __builtin_neon_vqsubuv2si (__simd64_int32_t  __a, __simd64_int32_t  __b);
__simd64_int32_t __builtin_neon_vraddhnv2di(__simd128_int64_t, __simd128_int64_t);
__simd64_int32_t __builtin_neon_vraddhnv4si (__simd128_int32_t __a, __simd128_int32_t __b);
__simd64_int32_t __builtin_neon_vrecpev2si (__simd64_int32_t  __a);
__simd64_int32_t __builtin_neon_vreinterpretv2sfdi (int  __a);
__simd64_int32_t __builtin_neon_vreinterpretv2sfv2sf (__simd64_float32_t __a);
__simd64_int32_t __builtin_neon_vreinterpretv2sfv2si (__simd64_int32_t  __a);
__simd64_int32_t __builtin_neon_vreinterpretv2sfv4hi (__simd64_int32_t __a);
__simd64_int32_t __builtin_neon_vreinterpretv2sfv8qi (__simd64_int32_t  __a);
__simd64_int32_t __builtin_neon_vreinterpretv2sidi (int  __a);
__simd64_int32_t __builtin_neon_vreinterpretv2siv2sf (__simd64_float32_t __a);
__simd64_int32_t __builtin_neon_vreinterpretv2siv2si (__simd64_int32_t  __a);
__simd64_int32_t __builtin_neon_vreinterpretv2siv4hi (__simd64_int32_t __a);
__simd64_int32_t __builtin_neon_vreinterpretv2siv8qi (__simd64_int32_t  __a);
__simd64_int32_t __builtin_neon_vreinterpretv4hidi (int  __a);
__simd64_int32_t __builtin_neon_vreinterpretv4hiv2sf (__simd64_float32_t __a);
__simd64_int32_t __builtin_neon_vreinterpretv4hiv2si (__simd64_int32_t  __a);
__simd64_int32_t __builtin_neon_vreinterpretv4hiv4hi (__simd64_int32_t __a);
__simd64_int32_t __builtin_neon_vreinterpretv4hiv8qi (__simd64_int32_t  __a);
__simd64_int32_t __builtin_neon_vreinterpretv8qidi (int  __a);
__simd64_int32_t __builtin_neon_vreinterpretv8qiv2sf (__simd64_float32_t __a);
__simd64_int32_t __builtin_neon_vreinterpretv8qiv2si (__simd64_int32_t __a);
__simd64_int32_t __builtin_neon_vreinterpretv8qiv4hi (__simd64_int32_t  __a);
__simd64_int32_t __builtin_neon_vreinterpretv8qiv8qi (__simd64_int32_t __a);
__simd64_int32_t __builtin_neon_vrhaddsv2si (__simd64_int32_t __a, __simd64_int32_t __b);
__simd64_int32_t __builtin_neon_vrhadduv2si (__simd64_int32_t  __a, __simd64_int32_t  __b);
__simd64_int32_t __builtin_neon_vrshlsv2si (__simd64_int32_t __a, __simd64_int32_t __b);
__simd64_int32_t __builtin_neon_vrshluv2si (__simd64_int32_t __a, __simd64_int32_t __b);
__simd64_int32_t __builtin_neon_vrshrn_nv2di(__simd128_int64_t, int);
__simd64_int32_t __builtin_neon_vrshrn_nv4si (__simd128_int32_t __a, const int __b);
__simd64_int32_t __builtin_neon_vrshrs_nv2si (__simd64_int32_t __a, const int __b);
__simd64_int32_t __builtin_neon_vrshru_nv2si (__simd64_int32_t __a, const int __b);
__simd64_int32_t __builtin_neon_vrsqrtev2si (__simd64_int32_t  __a);
__simd64_int32_t __builtin_neon_vrsras_nv2si (__simd64_int32_t __a, __simd64_int32_t __b, const int __c);
__simd64_int32_t __builtin_neon_vrsrau_nv2si (__simd64_int32_t __a, __simd64_int32_t __b, const int __c);
__simd64_int32_t __builtin_neon_vrsubhnv2di(__simd128_int64_t, __simd128_int64_t);
__simd64_int32_t __builtin_neon_vrsubhnv4si (__simd128_int32_t __a, __simd128_int32_t __b);
__simd64_int32_t __builtin_neon_vset_lanev2si (int __a, __simd64_int32_t __a, const int __c);
__simd64_int32_t __builtin_neon_vshl_nv2si (__simd64_int32_t __a, const int __b);
__simd64_int32_t __builtin_neon_vshlsv2si (__simd64_int32_t __a, __simd64_int32_t __b);
__simd64_int32_t __builtin_neon_vshluv2si (__simd64_int32_t __a, __simd64_int32_t __b);
__simd64_int32_t __builtin_neon_vshrn_nv2di(__simd128_int64_t, int);
__simd64_int32_t __builtin_neon_vshrn_nv4si (__simd128_int32_t __a, const int __b);
__simd64_int32_t __builtin_neon_vshrs_nv2si (__simd64_int32_t __a, const int __b);
__simd64_int32_t __builtin_neon_vshru_nv2si (__simd64_int32_t __a, const int __b);
__simd64_int32_t __builtin_neon_vsli_nv2si (__simd64_int32_t __a, __simd64_int32_t __b, const int __c);
__simd64_int32_t __builtin_neon_vsras_nv2si (__simd64_int32_t __a, __simd64_int32_t __b, const int __c);
__simd64_int32_t __builtin_neon_vsrau_nv2si (__simd64_int32_t __a, __simd64_int32_t __b, const int __c);
__simd64_int32_t __builtin_neon_vsri_nv2si (__simd64_int32_t __a, __simd64_int32_t __b, const int __c);
void __builtin_neon_vst1di(long long int*, long long int);
void __builtin_neon_vst1_lanev2si(int*, __simd64_int32_t, int);
void __builtin_neon_vst1v2si(int*, __simd64_int32_t);
__simd64_int32_t __builtin_neon_vsubhnv2di(__simd128_int64_t, __simd128_int64_t);
__simd64_int32_t __builtin_neon_vsubhnv4si (__simd128_int32_t __a, __simd128_int32_t __b);
__simd64_int32_t __builtin_neon_vtstv2si (__simd64_int32_t __a, __simd64_int32_t __b);
__simd64_int8_t __builtin_neon_vabasv8qi(__simd64_int8_t, __simd64_int8_t, __simd64_int8_t);
__simd64_int8_t __builtin_neon_vabauv8qi(__simd64_int8_t, __simd64_int8_t, __simd64_int8_t);
__simd64_int8_t __builtin_neon_vabdsv8qi(__simd64_int8_t, __simd64_int8_t);
__simd64_int8_t __builtin_neon_vabduv8qi(__simd64_int8_t, __simd64_int8_t);
__simd64_int8_t __builtin_neon_vabsv8qi(__simd64_int8_t);
__simd64_int8_t __builtin_neon_vaddhnv8hi(__simd128_int16_t, __simd128_int16_t);
__simd64_int8_t __builtin_neon_vbslv8qi(__simd64_int8_t, __simd64_int8_t, __simd64_int8_t);
__simd64_int8_t __builtin_neon_vceqv8qi(__simd64_int8_t, __simd64_int8_t);
__simd64_int8_t __builtin_neon_vcgeuv8qi(__simd64_int8_t, __simd64_int8_t);
__simd64_int8_t __builtin_neon_vcgev8qi(__simd64_int8_t, __simd64_int8_t);
__simd64_int8_t __builtin_neon_vcgtuv8qi(__simd64_int8_t, __simd64_int8_t);
__simd64_int8_t __builtin_neon_vcgtv8qi(__simd64_int8_t, __simd64_int8_t);
__simd64_int8_t __builtin_neon_vclsv8qi(__simd64_int8_t);
__simd64_int8_t __builtin_neon_vclzv8qi(__simd64_int8_t);
__simd64_int8_t __builtin_neon_vcntv8qi(__simd64_int8_t);
__simd64_int8_t __builtin_neon_vdup_lanev8qi(__simd64_int8_t, int);
__simd64_int8_t __builtin_neon_vextv8qi(__simd64_int8_t, __simd64_int8_t, int);
__simd64_int8_t __builtin_neon_vget_highv16qi(__simd128_int8_t);
__simd64_int8_t __builtin_neon_vget_lowv16qi(__simd128_int8_t);
__simd64_int8_t __builtin_neon_vhaddsv8qi(__simd64_int8_t, __simd64_int8_t);
__simd64_int8_t __builtin_neon_vhadduv8qi(__simd64_int8_t, __simd64_int8_t);
__simd64_int8_t __builtin_neon_vhsubsv8qi(__simd64_int8_t, __simd64_int8_t);
__simd64_int8_t __builtin_neon_vhsubuv8qi(__simd64_int8_t, __simd64_int8_t);
__simd64_int8_t __builtin_neon_vld1_lanev8qi(const signed char*, __simd64_int8_t, int);
__simd64_int8_t __builtin_neon_vmaxsv8qi(__simd64_int8_t, __simd64_int8_t);
__simd64_int8_t __builtin_neon_vmaxuv8qi(__simd64_int8_t, __simd64_int8_t);
__simd64_int8_t __builtin_neon_vminsv8qi(__simd64_int8_t, __simd64_int8_t);
__simd64_int8_t __builtin_neon_vminuv8qi(__simd64_int8_t, __simd64_int8_t);
__simd64_int8_t __builtin_neon_vmlav8qi(__simd64_int8_t, __simd64_int8_t, __simd64_int8_t);
__simd64_int8_t __builtin_neon_vmlsv8qi(__simd64_int8_t, __simd64_int8_t, __simd64_int8_t);
__simd64_int8_t __builtin_neon_vmovnv8hi(__simd128_int16_t);
__simd64_int8_t __builtin_neon_vmulpv8qi(__simd64_int8_t, __simd64_int8_t);
__simd64_int8_t __builtin_neon_vmvnv8qi(__simd64_int8_t);
__simd64_int8_t __builtin_neon_vnegv8qi(__simd64_int8_t);
__simd64_int8_t __builtin_neon_vpaddv8qi(__simd64_int8_t, __simd64_int8_t);
__simd64_int8_t __builtin_neon_vpmaxsv8qi(__simd64_int8_t, __simd64_int8_t);
__simd64_int8_t __builtin_neon_vpmaxuv8qi(__simd64_int8_t, __simd64_int8_t);
__simd64_int8_t __builtin_neon_vpminsv8qi(__simd64_int8_t, __simd64_int8_t);
__simd64_int8_t __builtin_neon_vpminuv8qi(__simd64_int8_t, __simd64_int8_t);
__simd64_int8_t __builtin_neon_vqabsv8qi(__simd64_int8_t);
__simd64_int8_t __builtin_neon_vqaddsv8qi(__simd64_int8_t, __simd64_int8_t);
__simd64_int8_t __builtin_neon_vqadduv8qi(__simd64_int8_t, __simd64_int8_t);
__simd64_int8_t __builtin_neon_vqmovnsv8hi(__simd128_int16_t);
__simd64_int8_t __builtin_neon_vqmovnuv8hi(__simd128_int16_t);
__simd64_int8_t __builtin_neon_vqmovunv8hi(__simd128_int16_t);
__simd64_int8_t __builtin_neon_vqnegv8qi(__simd64_int8_t);
__simd64_int8_t __builtin_neon_vqrshlsv8qi(__simd64_int8_t, __simd64_int8_t);
__simd64_int8_t __builtin_neon_vqrshluv8qi(__simd64_int8_t, __simd64_int8_t);
__simd64_int8_t __builtin_neon_vqrshrns_nv8hi(__simd128_int16_t, int);
__simd64_int8_t __builtin_neon_vqrshrnu_nv8hi(__simd128_int16_t, int);
__simd64_int8_t __builtin_neon_vqrshrun_nv8hi(__simd128_int16_t, int);
__simd64_int8_t __builtin_neon_vqshl_s_nv8qi(__simd64_int8_t, int);
__simd64_int8_t __builtin_neon_vqshlsv8qi(__simd64_int8_t, __simd64_int8_t);
__simd64_int8_t __builtin_neon_vqshl_u_nv8qi(__simd64_int8_t, int);
__simd64_int8_t __builtin_neon_vqshlu_nv8qi(__simd64_int8_t, int);
__simd64_int8_t __builtin_neon_vqshluv8qi(__simd64_int8_t, __simd64_int8_t);
__simd64_int8_t __builtin_neon_vqshrns_nv8hi(__simd128_int16_t, int);
__simd64_int8_t __builtin_neon_vqshrnu_nv8hi(__simd128_int16_t, int);
__simd64_int8_t __builtin_neon_vqshrun_nv8hi(__simd128_int16_t, int);
__simd64_int8_t __builtin_neon_vqsubsv8qi(__simd64_int8_t, __simd64_int8_t);
__simd64_int8_t __builtin_neon_vqsubuv8qi(__simd64_int8_t, __simd64_int8_t);
__simd64_int8_t __builtin_neon_vraddhnv8hi(__simd128_int16_t, __simd128_int16_t);
__simd64_int8_t __builtin_neon_vrhaddsv8qi(__simd64_int8_t, __simd64_int8_t);
__simd64_int8_t __builtin_neon_vrhadduv8qi(__simd64_int8_t, __simd64_int8_t);
__simd64_int8_t __builtin_neon_vrshlsv8qi(__simd64_int8_t, __simd64_int8_t);
__simd64_int8_t __builtin_neon_vrshluv8qi(__simd64_int8_t, __simd64_int8_t);
__simd64_int8_t __builtin_neon_vrshrn_nv8hi(__simd128_int16_t, int);
__simd64_int8_t __builtin_neon_vrshrs_nv8qi(__simd64_int8_t, int);
__simd64_int8_t __builtin_neon_vrshru_nv8qi(__simd64_int8_t, int);
__simd64_int8_t __builtin_neon_vrsras_nv8qi(__simd64_int8_t, __simd64_int8_t, int);
__simd64_int8_t __builtin_neon_vrsrau_nv8qi(__simd64_int8_t, __simd64_int8_t, int);
__simd64_int8_t __builtin_neon_vrsubhnv8hi(__simd128_int16_t, __simd128_int16_t);
__simd64_int8_t __builtin_neon_vset_lanev8qi(signed char, __simd64_int8_t, int);
__simd64_int8_t __builtin_neon_vshl_nv8qi(__simd64_int8_t, int);
__simd64_int8_t __builtin_neon_vshlsv8qi(__simd64_int8_t, __simd64_int8_t);
__simd64_int8_t __builtin_neon_vshluv8qi(__simd64_int8_t, __simd64_int8_t);
__simd64_int8_t __builtin_neon_vshrn_nv8hi(__simd128_int16_t, int);
__simd64_int8_t __builtin_neon_vshrs_nv8qi(__simd64_int8_t, int);
__simd64_int8_t __builtin_neon_vshru_nv8qi(__simd64_int8_t, int);
__simd64_int8_t __builtin_neon_vsli_nv8qi(__simd64_int8_t, __simd64_int8_t, int);
__simd64_int8_t __builtin_neon_vsras_nv8qi(__simd64_int8_t, __simd64_int8_t, int);
__simd64_int8_t __builtin_neon_vsrau_nv8qi(__simd64_int8_t, __simd64_int8_t, int);
__simd64_int8_t __builtin_neon_vsri_nv8qi(__simd64_int8_t, __simd64_int8_t, int);
__simd64_int8_t __builtin_neon_vsubhnv8hi(__simd128_int16_t, __simd128_int16_t);
__simd64_int8_t __builtin_neon_vtbl1v8qi(__simd64_int8_t, __simd64_int8_t);
__simd64_int8_t __builtin_neon_vtbl2v8qi(__builtin_neon_ti, __simd64_int8_t);
__simd64_int8_t __builtin_neon_vtbl3v8qi(__builtin_neon_ei, __simd64_int8_t);
__simd64_int8_t __builtin_neon_vtbl4v8qi(__builtin_neon_oi, __simd64_int8_t);
__simd64_int8_t __builtin_neon_vtbx1v8qi(__simd64_int8_t, __simd64_int8_t, __simd64_int8_t);
__simd64_int8_t __builtin_neon_vtbx2v8qi(__simd64_int8_t, __builtin_neon_ti, __simd64_int8_t);
__simd64_int8_t __builtin_neon_vtbx3v8qi(__simd64_int8_t, __builtin_neon_ei, __simd64_int8_t);
__simd64_int8_t __builtin_neon_vtbx4v8qi(__simd64_int8_t, __builtin_neon_oi, __simd64_int8_t);
__simd64_int8_t __builtin_neon_vtstv8qi(__simd64_int8_t, __simd64_int8_t);
__simd64_uint32_t __builtin_neon_udot_lanev8qi_uuuus(__simd64_uint32_t, __simd64_uint8_t, __simd64_uint8_t, int);
__simd64_uint32_t __builtin_neon_udotv8qi_uuuu(__simd64_uint32_t, __simd64_uint8_t, __simd64_uint8_t);
unsigned int __builtin_neon_vget_laneuv2si (__simd64_int32_t __a, const int __b);
unsigned int __builtin_neon_vget_laneuv4si (__simd128_int32_t  __a, const int __b);
unsigned int __builtin_neon_vpadaluv2si (int  __a, __simd64_int32_t  __b);
unsigned int __builtin_neon_vpaddluv2si (__simd64_int32_t  __a);
unsigned int __builtin_neon_vqaddudi (int  __a, int  __b);
unsigned int __builtin_neon_vqrshludi (int  __a, int __b);
unsigned int __builtin_neon_vqshludi (int  __a, int __b);
unsigned int __builtin_neon_vqshl_u_ndi (int  __a, const int __b);
unsigned int __builtin_neon_vqshlu_ndi (int __a, const int __b);
unsigned int __builtin_neon_vrshludi (int  __a, int __b);
unsigned int __builtin_neon_vrshru_ndi (int  __a, const int __b);
unsigned int __builtin_neon_vrsrau_ndi (int  __a, int  __b, const int __c);
unsigned int __builtin_neon_vshludi (int  __a, int __b);
unsigned int __builtin_neon_vshru_ndi (int  __a, const int __b);
unsigned int __builtin_neon_vsrau_ndi (int  __a, int  __b, const int __c);
void __builtin_neon_vst1_lanev16qi(signed char*, __simd128_int8_t, int);
void __builtin_neon_vst1_lanev2di(long long int*, __simd128_int64_t, int);
void __builtin_neon_vst1_lanev4hf(cov_fp16_t*, __simd64_float16_t, int);
void __builtin_neon_vst1_lanev4hi(short int*, __simd64_int16_t, int);
void __builtin_neon_vst1_lanev8hf(cov_fp16_t*, __simd128_float16_t, int);
void __builtin_neon_vst1_lanev8hi(short int*, __simd128_int16_t, int);
void __builtin_neon_vst1_lanev8qi(signed char*, __simd64_int8_t, int);
void __builtin_neon_vst1v16qi(signed char*, __simd128_int8_t);
void __builtin_neon_vst1v2di(long long int*, __simd128_int64_t);
void __builtin_neon_vst1v4hf(cov_fp16_t*, __simd64_float16_t);
void __builtin_neon_vst1v4hi(short int*, __simd64_int16_t);
void __builtin_neon_vst1v8hf(cov_fp16_t*, __simd128_float16_t);
void __builtin_neon_vst1v8hi(short int*, __simd128_int16_t);
void __builtin_neon_vst1v8qi(signed char*, __simd64_int8_t);
void __builtin_neon_vst2_lanev4hf(cov_fp16_t*, __builtin_neon_ti, int);
void __builtin_neon_vst2_lanev8hf(cov_fp16_t*, __builtin_neon_oi, int);
void __builtin_neon_vst2v4hf(cov_fp16_t*, __builtin_neon_ti);
void __builtin_neon_vst2v8hf(cov_fp16_t*, __builtin_neon_oi);
void __builtin_neon_vst3_lanev4hf(cov_fp16_t*, __builtin_neon_ei, int);
void __builtin_neon_vst3_lanev8hf(cov_fp16_t*, __builtin_neon_ci, int);
void __builtin_neon_vst3v4hf(cov_fp16_t*, __builtin_neon_ei);
void __builtin_neon_vst3v8hf(cov_fp16_t*, __builtin_neon_ci);
void __builtin_neon_vst4_lanev4hf(cov_fp16_t*, __builtin_neon_oi, int);
void __builtin_neon_vst4_lanev8hf(cov_fp16_t*, __builtin_neon_xi, int);
void __builtin_neon_vst4v4hf(cov_fp16_t*, __builtin_neon_oi);
void __builtin_neon_vst4v8hf(cov_fp16_t*, __builtin_neon_xi);

// -mfpu=vfpv2 -mfloat-abi=hard -mfpu=neon
__simd128_uint64_t __builtin_neon_bswapv2di_uu(__simd128_uint64_t);
__simd64_uint32_t __builtin_neon_bswapv2si_uu(__simd64_uint32_t);
__simd64_uint16_t __builtin_neon_bswapv4hi_uu(__simd64_uint16_t);
__simd128_uint32_t __builtin_neon_bswapv4si_uu(__simd128_uint32_t);
__simd128_uint16_t __builtin_neon_bswapv8hi_uu(__simd128_uint16_t);
__simd64_float32_t __builtin_neon_copysignfv2sf(__simd64_float32_t);
__simd128_float32_t __builtin_neon_copysignfv4sf(__simd128_float32_t);
__simd128_int32_t __builtin_neon_smmlav16qi(__simd128_int32_t, __simd128_int8_t, __simd128_int8_t);
__simd128_int32_t __builtin_neon_sudot_lanev16qi_sssus(__simd128_int32_t, __simd128_int8_t, __simd64_uint8_t, int);
__simd64_int32_t __builtin_neon_sudot_lanev8qi_sssus(__simd64_int32_t, __simd64_int8_t, __simd64_uint8_t, int);
__simd128_uint32_t __builtin_neon_ummlav16qi_uuuu(__simd128_uint32_t, __simd128_uint8_t, __simd128_uint8_t);
__simd128_int32_t __builtin_neon_usdot_lanev16qi_ssuss(__simd128_int32_t, __simd128_uint8_t, __simd64_int8_t, int);
__simd64_int32_t __builtin_neon_usdot_lanev8qi_ssuss(__simd64_int32_t, __simd64_uint8_t, __simd64_int8_t, int);
__simd64_int32_t __builtin_neon_usdotv8qi_ssus(__simd64_int32_t, __simd64_uint8_t, __simd64_int8_t);
__simd128_int32_t __builtin_neon_usmmlav16qi_ssus(__simd128_int32_t, __simd128_uint8_t, __simd128_int8_t);
float __builtin_neon_vbfcvtbf(__bf16);
__simd128_float32_t __builtin_neon_vbfcvt_highv8bf(__simd128_bfloat16_t);
__bf16 __builtin_neon_vbfcvtsf(float);
__simd128_float32_t __builtin_neon_vbfcvtv4bf(__simd64_bfloat16_t);
__simd128_bfloat16_t __builtin_neon_vbfcvtv4sf_highv8bf(__simd128_bfloat16_t, __simd128_float32_t);
__simd64_bfloat16_t __builtin_neon_vbfcvtv4sfv4bf(__simd128_float32_t);
__simd128_bfloat16_t __builtin_neon_vbfcvtv4sfv8bf(__simd128_float32_t);
__simd128_float32_t __builtin_neon_vbfcvtv8bf(__simd128_bfloat16_t);
__simd64_float32_t __builtin_neon_vbfdot_lanev4bfv2sf(__simd64_float32_t, __simd64_bfloat16_t, __simd64_bfloat16_t, int);
__simd128_float32_t __builtin_neon_vbfdot_lanev4bfv4sf(__simd128_float32_t, __simd128_bfloat16_t, __simd64_bfloat16_t, int);
__simd64_float32_t __builtin_neon_vbfdot_lanev8bfv2sf(__simd64_float32_t, __simd64_bfloat16_t, __simd128_bfloat16_t, int);
__simd128_float32_t __builtin_neon_vbfdot_lanev8bfv4sf(__simd128_float32_t, __simd128_bfloat16_t, __simd128_bfloat16_t, int);
__simd64_float32_t __builtin_neon_vbfdotv2sf(__simd64_float32_t, __simd64_bfloat16_t, __simd64_bfloat16_t);
__simd128_float32_t __builtin_neon_vbfdotv4sf(__simd128_float32_t, __simd128_bfloat16_t, __simd128_bfloat16_t);
__simd64_float32_t __builtin_neon_vcadd270v2sf(__simd64_float32_t, __simd64_float32_t);
__simd64_float16_t __builtin_neon_vcadd270v4hf(__simd64_float16_t, __simd64_float16_t);
__simd128_float32_t __builtin_neon_vcadd270v4sf(__simd128_float32_t, __simd128_float32_t);
__simd128_float16_t __builtin_neon_vcadd270v8hf(__simd128_float16_t, __simd128_float16_t);
__simd128_bfloat16_t __builtin_neon_vcombinev4bf(__simd64_bfloat16_t, __simd64_bfloat16_t);
__simd64_int32_t __builtin_neon_vcvtauv2sfv2si(__simd64_float32_t);
__simd128_int32_t __builtin_neon_vcvtauv4sfv4si(__simd128_float32_t);
__simd64_int32_t __builtin_neon_vcvtav2sfv2si(__simd64_float32_t);
__simd128_int32_t __builtin_neon_vcvtav4sfv4si(__simd128_float32_t);
__simd64_int32_t __builtin_neon_vcvtmuv2sfv2si(__simd64_float32_t);
__simd128_int32_t __builtin_neon_vcvtmuv4sfv4si(__simd128_float32_t);
__simd64_int32_t __builtin_neon_vcvtmv2sfv2si(__simd64_float32_t);
__simd128_int32_t __builtin_neon_vcvtmv4sfv4si(__simd128_float32_t);
__simd64_int32_t __builtin_neon_vcvtpuv2sfv2si(__simd64_float32_t);
__simd128_int32_t __builtin_neon_vcvtpuv4sfv4si(__simd128_float32_t);
__simd64_int32_t __builtin_neon_vcvtpv2sfv2si(__simd64_float32_t);
__simd128_int32_t __builtin_neon_vcvtpv4sfv4si(__simd128_float32_t);
__simd64_bfloat16_t __builtin_neon_vdup_lanev4bf(__simd64_bfloat16_t, int);
__simd128_bfloat16_t __builtin_neon_vdup_lanev8bf(__simd64_bfloat16_t, int);
__simd64_bfloat16_t __builtin_neon_vdup_nv4bf(__bf16);
__simd128_bfloat16_t __builtin_neon_vdup_nv8bf(__bf16);
__simd128_float32_t __builtin_neon_vfmab_laneqv8bf(__simd128_float32_t, __simd128_bfloat16_t, __simd128_bfloat16_t, int);
__simd128_float32_t __builtin_neon_vfmab_lanev8bf(__simd128_float32_t, __simd128_bfloat16_t, __simd64_bfloat16_t, int);
__simd128_float32_t __builtin_neon_vfmabv8bf(__simd128_float32_t, __simd128_bfloat16_t, __simd128_bfloat16_t);
__simd128_float32_t __builtin_neon_vfmat_laneqv8bf(__simd128_float32_t, __simd128_bfloat16_t, __simd128_bfloat16_t, int);
__simd128_float32_t __builtin_neon_vfmat_lanev8bf(__simd128_float32_t, __simd128_bfloat16_t, __simd64_bfloat16_t, int);
__simd128_float32_t __builtin_neon_vfmatv8bf(__simd128_float32_t, __simd128_bfloat16_t, __simd128_bfloat16_t);
__simd64_bfloat16_t __builtin_neon_vget_highv8bf(__simd128_bfloat16_t);
__simd64_bfloat16_t __builtin_neon_vget_lowv8bf(__simd128_bfloat16_t);
__builtin_neon_ti __builtin_neon_vld2_dupv4bf(const __bf16*);
__builtin_neon_oi __builtin_neon_vld2_dupv8bf(const __bf16*);
__builtin_neon_ti __builtin_neon_vld2v4bf(const __bf16*);
__builtin_neon_oi __builtin_neon_vld2v8bf(const __bf16*);
__builtin_neon_ei __builtin_neon_vld3_dupv4bf(const __bf16*);
__builtin_neon_ci __builtin_neon_vld3_dupv8bf(const __bf16*);
__builtin_neon_ei __builtin_neon_vld3v4bf(const __bf16*);
__builtin_neon_ci __builtin_neon_vld3v8bf(const __bf16*);
__builtin_neon_oi __builtin_neon_vld4_dupv4bf(const __bf16*);
__builtin_neon_xi __builtin_neon_vld4_dupv8bf(const __bf16*);
__builtin_neon_oi __builtin_neon_vld4v4bf(const __bf16*);
__builtin_neon_xi __builtin_neon_vld4v8bf(const __bf16*);
__simd128_float32_t __builtin_neon_vmmlav8bf(__simd128_float32_t, __simd128_bfloat16_t, __simd128_bfloat16_t);
__simd128_int64_t __builtin_neon_vmullpv2si(__simd64_int32_t, __simd64_int32_t);
__simd128_int32_t __builtin_neon_vmullpv4hi(__simd64_int16_t, __simd64_int16_t);
__simd128_int8_t __builtin_neon_vrev16v16qi(__simd128_int8_t);
__simd64_int8_t __builtin_neon_vrev16v8qi(__simd64_int8_t);
__simd128_int8_t __builtin_neon_vrev32v16qi(__simd128_int8_t);
__simd64_int16_t __builtin_neon_vrev32v4hi(__simd64_int16_t);
__simd128_int16_t __builtin_neon_vrev32v8hi(__simd128_int16_t);
__simd64_int8_t __builtin_neon_vrev32v8qi(__simd64_int8_t);
__simd128_int8_t __builtin_neon_vrev64v16qi(__simd128_int8_t);
__simd64_float32_t __builtin_neon_vrev64v2sf(__simd64_float32_t);
__simd64_int32_t __builtin_neon_vrev64v2si(__simd64_int32_t);
__simd64_int16_t __builtin_neon_vrev64v4hi(__simd64_int16_t);
__simd128_float32_t __builtin_neon_vrev64v4sf(__simd128_float32_t);
__simd128_int32_t __builtin_neon_vrev64v4si(__simd128_int32_t);
__simd128_int16_t __builtin_neon_vrev64v8hi(__simd128_int16_t);
__simd64_int8_t __builtin_neon_vrev64v8qi(__simd64_int8_t);
void __builtin_neon_vst2v4bf(__bf16*, __builtin_neon_ti);
void __builtin_neon_vst2v8bf(__bf16*, __builtin_neon_oi);
void __builtin_neon_vst3v4bf(__bf16*, __builtin_neon_ei);
void __builtin_neon_vst3v8bf(__bf16*, __builtin_neon_ci);
void __builtin_neon_vst4v4bf(__bf16*, __builtin_neon_oi);
void __builtin_neon_vst4v8bf(__bf16*, __builtin_neon_xi);

#endif // __ARM_NEON__

/* aarch64 support */
#ifdef __aarch64__
typedef int __builtin_aarch64_simd_qi __attribute__((mode(QI)));
typedef int __builtin_aarch64_simd_hi __attribute__((mode(HI)));
typedef int __builtin_aarch64_simd_si __attribute__((mode(SI)));
typedef int __builtin_aarch64_simd_di __attribute__((mode(DI)));
typedef float __builtin_aarch64_simd_bf __attribute__((mode(BF)));
typedef int __builtin_aarch64_simd_poly8 __attribute__((mode(QI)));
typedef int __builtin_aarch64_simd_poly16 __attribute__((mode(HI)));
typedef int __builtin_aarch64_simd_poly64 __attribute__ ((mode (DI)));
typedef unsigned int __builtin_aarch64_simd_poly128 __attribute__ ((mode (TI)));
typedef float __builtin_aarch64_simd_df __attribute__ ((mode (DF)));
typedef float __builtin_aarch64_simd_sf __attribute__ ((mode (SF)));
typedef unsigned int __builtin_aarch64_simd_udi __attribute__((mode(DI)));
typedef unsigned int __builtin_aarch64_simd_uqi __attribute__((mode(QI)));
typedef unsigned int __builtin_aarch64_simd_uhi __attribute__((mode(HI)));
typedef unsigned int __builtin_aarch64_simd_usi __attribute__((mode(SI)));
typedef float __builtin_aarch64_simd_hf __attribute__((mode(HF)));
typedef __builtin_aarch64_simd_hf __fp16;
typedef int __Poly64_t __attribute__((mode(DI)));
typedef __coverity_decimal long long __Poly64x1_t __attribute__((vector_size (8)));
typedef int __Poly128_t __attribute__((mode(TI)));

unsigned int __builtin_aarch64_get_fpcr();
void __builtin_aarch64_set_fpcr(unsigned int);
unsigned int __builtin_aarch64_get_fpsr();
void __builtin_aarch64_set_fpsr(unsigned int);
//CMPCPP-6477
typedef char __Int8x8_t  __attribute__ ((vector_size (8)));
typedef short __Int16x4_t __attribute__ ((vector_size (8)));
typedef int __Int32x2_t __attribute__ ((vector_size (8)));
typedef long __Int64x1_t __attribute__((vector_size (8)));
typedef __coverity_float __fp16 __Float16x4_t __attribute__((vector_size (8)));
typedef float __Float32x2_t __attribute__((vector_size (8)));
typedef __coverity_decimal unsigned char __Poly8x8_t  __attribute__((vector_size (8)));
typedef __coverity_decimal short __Poly16x4_t __attribute__((vector_size (8)));
typedef unsigned char __Uint8x8_t __attribute__((vector_size (8)));
typedef unsigned short __Uint16x4_t __attribute__((vector_size (8)));
typedef unsigned int __Uint32x2_t __attribute__((vector_size (8)));
typedef double __Float64x1_t __attribute__((vector_size (8)));
typedef unsigned long __Uint64x1_t __attribute__((vector_size (8)));
typedef char __Int8x16_t __attribute__((vector_size (16)));
typedef short __Int16x8_t __attribute__((vector_size (16)));
typedef int __Int32x4_t __attribute__((vector_size (16)));
typedef long __Int64x2_t __attribute__((vector_size (16)));
typedef __coverity_float __fp16 __Float16x8_t __attribute__((vector_size (16)));
typedef __coverity_float __bf16 __Bfloat16x8_t __attribute__((vector_size (16)));
typedef __coverity_float __bf16 __Bfloat16x4_t __attribute__((vector_size (8)));
typedef float __Float32x4_t __attribute__((vector_size (16)));
typedef double __Float64x2_t __attribute__((vector_size (16)));
typedef __coverity_decimal unsigned char __Poly8x16_t __attribute__((vector_size (16)));
typedef __coverity_decimal unsigned short __Poly16x8_t __attribute__((vector_size (16)));
typedef __coverity_decimal long __Poly64x2_t __attribute__((vector_size (16)));
typedef unsigned char __Uint8x16_t __attribute__((vector_size (16)));
typedef unsigned short __Uint16x8_t __attribute__((vector_size (16)));
typedef unsigned int __Uint32x4_t __attribute__((vector_size (16)));
typedef unsigned long __Uint64x2_t  __attribute__((vector_size (16)));
typedef unsigned char __Poly8_t;
typedef unsigned short __Poly16_t;

typedef int __builtin_aarch64_simd_oi __attribute__((mode(OI)));
typedef int __builtin_aarch64_simd_ci __attribute__((mode(CI)));
typedef int __builtin_aarch64_simd_xi __attribute__((mode(XI)));

__Int8x8_t __builtin_aarch64_tbl3v8qi(__builtin_aarch64_simd_oi, __Int8x8_t);
__Float32x2_t __builtin_aarch64_absv2sf(__Float32x2_t);
__Int8x8_t __builtin_aarch64_absv8qi(__Int8x8_t);
__Int16x4_t __builtin_aarch64_absv4hi(__Int16x4_t);
__Int32x2_t __builtin_aarch64_absv2si(__Int32x2_t);
int __builtin_aarch64_absdi(int);
__Float32x4_t __builtin_aarch64_absv4sf(__Float32x4_t);
__Float64x2_t __builtin_aarch64_absv2df(__Float64x2_t);
__Int8x16_t __builtin_aarch64_absv16qi(__Int8x16_t);
int __builtin_aarch64_im_lane_boundsi(int, int, int);
__Int64x1_t __builtin_aarch64_get_dregoidi(__builtin_aarch64_simd_oi, int);
float __builtin_aarch64_get_dregoidf(__builtin_aarch64_simd_oi, int);
__Int8x8_t __builtin_aarch64_get_dregoiv8qi(__builtin_aarch64_simd_oi, int);
__Int16x4_t __builtin_aarch64_get_dregoiv4hi(__builtin_aarch64_simd_oi, int);
__Int32x2_t __builtin_aarch64_get_dregoiv2si(__builtin_aarch64_simd_oi, int);
__Float16x4_t __builtin_aarch64_get_dregoiv4hf(__builtin_aarch64_simd_oi, int);
__Float32x2_t __builtin_aarch64_get_dregoiv2sf(__builtin_aarch64_simd_oi, int);
__Int8x16_t __builtin_aarch64_get_qregoiv16qi(__builtin_aarch64_simd_oi, const int);
__Int16x8_t __builtin_aarch64_get_qregoiv8hi(__builtin_aarch64_simd_oi, const int);
__Int32x4_t __builtin_aarch64_get_qregoiv4si(__builtin_aarch64_simd_oi, const int);
__Int64x2_t __builtin_aarch64_get_qregoiv2di(__builtin_aarch64_simd_oi, const int);
__Float16x8_t __builtin_aarch64_get_qregoiv8hf(__builtin_aarch64_simd_oi, const int);
__Float32x4_t __builtin_aarch64_get_qregoiv4sf(__builtin_aarch64_simd_oi, const int);
__Float64x2_t __builtin_aarch64_get_qregoiv2df(__builtin_aarch64_simd_oi, const int);
__builtin_aarch64_simd_oi __builtin_aarch64_set_qregoiv8hf(__builtin_aarch64_simd_oi, __Float16x8_t, int);
__builtin_aarch64_simd_oi __builtin_aarch64_set_qregoiv4sf(__builtin_aarch64_simd_oi, __Float32x4_t, int);
__builtin_aarch64_simd_oi __builtin_aarch64_set_qregoiv2df(__builtin_aarch64_simd_oi, __Float64x2_t, int);
__builtin_aarch64_simd_oi __builtin_aarch64_set_qregoiv16qi(__builtin_aarch64_simd_oi, __Int8x16_t, int);
__builtin_aarch64_simd_oi __builtin_aarch64_set_qregoiv8hi(__builtin_aarch64_simd_oi, __Int16x8_t, int);
__builtin_aarch64_simd_oi __builtin_aarch64_set_qregoiv4si(__builtin_aarch64_simd_oi, __Int32x4_t, int);
__builtin_aarch64_simd_oi __builtin_aarch64_set_qregoiv2di(__builtin_aarch64_simd_oi, __Int64x2_t, int);
__builtin_aarch64_simd_ci __builtin_aarch64_ld3di(const __builtin_aarch64_simd_di*);
__Int64x1_t __builtin_aarch64_get_dregcidi(__builtin_aarch64_simd_ci, int);
float __builtin_aarch64_get_dregcidf(__builtin_aarch64_simd_ci, int);
__Int8x8_t __builtin_aarch64_get_dregciv8qi(__builtin_aarch64_simd_ci, int);
__Int16x4_t __builtin_aarch64_get_dregciv4hi(__builtin_aarch64_simd_ci, int);
__Int32x2_t __builtin_aarch64_get_dregciv2si(__builtin_aarch64_simd_ci, int);
__Float16x4_t __builtin_aarch64_get_dregciv4hf(__builtin_aarch64_simd_ci, int);
__Float32x2_t __builtin_aarch64_get_dregciv2sf(__builtin_aarch64_simd_ci, int);
__Int8x16_t __builtin_aarch64_get_qregciv16qi(__builtin_aarch64_simd_ci, const int);
__Int16x8_t __builtin_aarch64_get_qregciv8hi(__builtin_aarch64_simd_ci, const int);
__Int32x4_t __builtin_aarch64_get_qregciv4si(__builtin_aarch64_simd_ci, const int);
__Int64x2_t __builtin_aarch64_get_qregciv2di(__builtin_aarch64_simd_ci, const int);
__Float16x8_t __builtin_aarch64_get_qregciv8hf(__builtin_aarch64_simd_ci, const int);
__Float32x4_t __builtin_aarch64_get_qregciv4sf(__builtin_aarch64_simd_ci, const int);
__Float64x2_t __builtin_aarch64_get_qregciv2df(__builtin_aarch64_simd_ci, const int);
__builtin_aarch64_simd_ci __builtin_aarch64_set_qregciv8hf(__builtin_aarch64_simd_ci, __Float16x8_t, int);
__builtin_aarch64_simd_ci __builtin_aarch64_set_qregciv4sf(__builtin_aarch64_simd_ci, __Float32x4_t, int);
__builtin_aarch64_simd_ci __builtin_aarch64_set_qregciv2df(__builtin_aarch64_simd_ci, __Float64x2_t, int);
__builtin_aarch64_simd_ci __builtin_aarch64_set_qregciv16qi(__builtin_aarch64_simd_ci, __Int8x16_t, int);
__builtin_aarch64_simd_ci __builtin_aarch64_set_qregciv8hi(__builtin_aarch64_simd_ci, __Int16x8_t, int);
__builtin_aarch64_simd_ci __builtin_aarch64_set_qregciv4si(__builtin_aarch64_simd_ci, __Int32x4_t, int);
__builtin_aarch64_simd_ci __builtin_aarch64_set_qregciv2di(__builtin_aarch64_simd_ci, __Int64x2_t, int);
__Int64x1_t __builtin_aarch64_get_dregxidi(__builtin_aarch64_simd_xi, int);
float __builtin_aarch64_get_dregxidf(__builtin_aarch64_simd_xi, int);
__Int8x8_t __builtin_aarch64_get_dregxiv8qi(__builtin_aarch64_simd_xi, int);
__Int16x4_t __builtin_aarch64_get_dregxiv4hi(__builtin_aarch64_simd_xi, int);
__Int32x2_t __builtin_aarch64_get_dregxiv2si(__builtin_aarch64_simd_xi, int);
__Float16x4_t __builtin_aarch64_get_dregxiv4hf(__builtin_aarch64_simd_xi, int);
__Float32x2_t __builtin_aarch64_get_dregxiv2sf(__builtin_aarch64_simd_xi, int);
__Int8x16_t __builtin_aarch64_get_qregxiv16qi(__builtin_aarch64_simd_xi, const int);
__Int16x8_t __builtin_aarch64_get_qregxiv8hi(__builtin_aarch64_simd_xi, const int);
__Int32x4_t __builtin_aarch64_get_qregxiv4si(__builtin_aarch64_simd_xi, const int);
__Int64x2_t __builtin_aarch64_get_qregxiv2di(__builtin_aarch64_simd_xi, const int);
__Float16x8_t __builtin_aarch64_get_qregxiv8hf(__builtin_aarch64_simd_xi, const int);
__Float32x4_t __builtin_aarch64_get_qregxiv4sf(__builtin_aarch64_simd_xi, const int);
__Float64x2_t __builtin_aarch64_get_qregxiv2df(__builtin_aarch64_simd_xi, const int);
__builtin_aarch64_simd_xi __builtin_aarch64_set_qregxiv8hf(__builtin_aarch64_simd_xi, __Float16x8_t, int);
__builtin_aarch64_simd_xi __builtin_aarch64_set_qregxiv4sf(__builtin_aarch64_simd_xi, __Float32x4_t, int);
__builtin_aarch64_simd_xi __builtin_aarch64_set_qregxiv2df(__builtin_aarch64_simd_xi, __Float64x2_t, int);
__builtin_aarch64_simd_xi __builtin_aarch64_set_qregxiv16qi(__builtin_aarch64_simd_xi, __Int8x16_t, int);
__builtin_aarch64_simd_xi __builtin_aarch64_set_qregxiv8hi(__builtin_aarch64_simd_xi, __Int16x8_t, int);
__builtin_aarch64_simd_xi __builtin_aarch64_set_qregxiv4si(__builtin_aarch64_simd_xi, __Int32x4_t, int);
__builtin_aarch64_simd_xi __builtin_aarch64_set_qregxiv2di(__builtin_aarch64_simd_xi, __Int64x2_t, int);
__Int32x4_t __builtin_aarch64_saddlv4hi(__Int16x4_t, __Int16x4_t);
__Int16x8_t __builtin_aarch64_saddlv8qi(__Int8x8_t, __Int8x8_t);
__Uint16x8_t __builtin_aarch64_uaddlv8qi(__Int8x8_t, __Int8x8_t);
__Int16x8_t __builtin_aarch64_saddl2v16qi(__Int8x16_t, __Int8x16_t);
__Uint16x8_t __builtin_aarch64_uaddl2v16qi(__Int8x16_t, __Int8x16_t);
__Int16x8_t __builtin_aarch64_saddwv8qi(__Int16x8_t, __Int8x8_t);
__Uint16x8_t __builtin_aarch64_uaddwv8qi(__Int16x8_t, __Int8x8_t);
__Int16x8_t __builtin_aarch64_saddw2v16qi(__Int16x8_t, __Int8x16_t);
__Uint16x8_t __builtin_aarch64_uaddw2v16qi(__Int16x8_t, __Int8x16_t);
__Int8x8_t __builtin_aarch64_shaddv8qi(__Int8x8_t, __Int8x8_t);
__Uint8x8_t __builtin_aarch64_uhaddv8qi(__Int8x8_t, __Int8x8_t);
__Int8x16_t __builtin_aarch64_shaddv16qi(__Int8x16_t, __Int8x16_t);
__Int16x8_t __builtin_aarch64_shaddv8hi(__Int16x8_t, __Int16x8_t);
__Uint8x16_t __builtin_aarch64_uhaddv16qi(__Int8x16_t, __Int8x16_t);
__Uint16x8_t __builtin_aarch64_uhaddv8hi(__Int16x8_t, __Int16x8_t);
__Int8x8_t __builtin_aarch64_srhaddv8qi(__Int8x8_t, __Int8x8_t);
__Uint8x8_t __builtin_aarch64_urhaddv8qi(__Int8x8_t, __Int8x8_t);
__Int8x16_t __builtin_aarch64_srhaddv16qi(__Int8x16_t, __Int8x16_t);
__Int16x8_t __builtin_aarch64_srhaddv8hi(__Int16x8_t, __Int16x8_t);
__Uint8x16_t __builtin_aarch64_urhaddv16qi(__Int8x16_t, __Int8x16_t);
__Uint16x8_t __builtin_aarch64_urhaddv8hi(__Int16x8_t, __Int16x8_t);
__Int8x8_t __builtin_aarch64_addhnv8hi(__Int16x8_t, __Int16x8_t);
__Int8x8_t __builtin_aarch64_raddhnv8hi(__Int16x8_t, __Int16x8_t);
__Int8x16_t __builtin_aarch64_addhn2v8hi(__Int8x8_t, __Int16x8_t, __Int16x8_t);
__Int16x8_t __builtin_aarch64_addhn2v4si(__Int16x4_t, __Int32x4_t, __Int32x4_t);
__Int8x16_t __builtin_aarch64_raddhn2v8hi(__Int8x8_t, __Int16x8_t, __Int16x8_t);
__Int16x8_t __builtin_aarch64_raddhn2v4si(__Int16x4_t, __Int32x4_t, __Int32x4_t);
__Poly8x8_t __builtin_aarch64_pmulv8qi(__Int8x8_t, __Int8x8_t);
__Poly8x16_t __builtin_aarch64_pmulv16qi(__Int8x16_t, __Int8x16_t);
__Int16x8_t __builtin_aarch64_ssublv8qi(__Int8x8_t, __Int8x8_t);
__Uint16x8_t __builtin_aarch64_usublv8qi(__Int8x8_t, __Int8x8_t);
__Float32x2_t __builtin_aarch64_smax_nanpv2sf(__Float32x2_t, __Float32x2_t);
__Int64x2_t __builtin_aarch64_saddlv2si(__Int32x2_t, __Int32x2_t);
__Uint32x4_t __builtin_aarch64_uaddlv4hi (__Int16x4_t, __Int16x4_t);
__Uint64x2_t __builtin_aarch64_uaddlv2si (__Int32x2_t, __Int32x2_t);
__Int32x4_t __builtin_aarch64_saddl2v8hi (__Int16x8_t, __Int16x8_t);
__Int64x2_t __builtin_aarch64_saddl2v4si (__Int32x4_t, __Int32x4_t);
__Uint32x4_t __builtin_aarch64_uaddl2v8hi (__Int16x8_t, __Int16x8_t);
__Uint64x2_t __builtin_aarch64_uaddl2v4si (__Int32x4_t, __Int32x4_t);
__Int32x4_t __builtin_aarch64_saddwv4hi (__Int32x4_t, __Int16x4_t);
__Int64x2_t __builtin_aarch64_saddwv2si (__Int64x2_t, __Int32x2_t);
__Uint32x4_t __builtin_aarch64_uaddwv4hi (__Int32x4_t, __Int16x4_t);
__Uint64x2_t __builtin_aarch64_uaddwv2si (__Int64x2_t, __Int32x2_t);
__Int32x4_t __builtin_aarch64_saddw2v8hi (__Int32x4_t, __Int16x8_t);
__Int64x2_t __builtin_aarch64_saddw2v4si (__Int64x2_t, __Int32x4_t);
__Uint32x4_t __builtin_aarch64_uaddw2v8hi (__Int32x4_t, __Int16x8_t);
__Uint64x2_t __builtin_aarch64_uaddw2v4si (__Int64x2_t, __Int32x4_t);
__Int16x4_t __builtin_aarch64_shaddv4hi (__Int16x4_t, __Int16x4_t);
__Int32x2_t __builtin_aarch64_shaddv2si (__Int32x2_t, __Int32x2_t);
__Uint16x4_t __builtin_aarch64_uhaddv4hi (__Int16x4_t, __Int16x4_t);
__Uint32x2_t __builtin_aarch64_uhaddv2si (__Int32x2_t, __Int32x2_t);
__Int32x4_t __builtin_aarch64_shaddv4si (__Int32x4_t, __Int32x4_t);
__Uint32x4_t __builtin_aarch64_uhaddv4si (__Int32x4_t, __Int32x4_t);
__Int16x4_t __builtin_aarch64_srhaddv4hi (__Int16x4_t, __Int16x4_t);
__Int32x2_t __builtin_aarch64_srhaddv2si (__Int32x2_t, __Int32x2_t);
__Uint16x4_t __builtin_aarch64_urhaddv4hi (__Int16x4_t, __Int16x4_t);
__Uint32x2_t __builtin_aarch64_urhaddv2si (__Int32x2_t, __Int32x2_t);
__Int32x4_t __builtin_aarch64_srhaddv4si (__Int32x4_t, __Int32x4_t);
__Uint32x4_t __builtin_aarch64_urhaddv4si (__Int32x4_t, __Int32x4_t);
__Int16x4_t __builtin_aarch64_addhnv4si (__Int32x4_t, __Int32x4_t);
__Int32x2_t __builtin_aarch64_addhnv2di (__Int64x2_t, __Int64x2_t);
__Uint32x2_t __builtin_aarch64_addhnv2di (__Int64x2_t, __Int64x2_t);
__Int16x4_t __builtin_aarch64_raddhnv4si (__Int32x4_t, __Int32x4_t);
__Int32x2_t __builtin_aarch64_raddhnv2di (__Int64x2_t, __Int64x2_t);
__Uint16x4_t __builtin_aarch64_raddhnv4si (__Int32x4_t, __Int32x4_t);
__Uint32x2_t __builtin_aarch64_raddhnv2di (__Int64x2_t, __Int64x2_t);
__Int32x4_t __builtin_aarch64_addhn2v2di (__Int32x2_t, __Int64x2_t, __Int64x2_t);
__Uint32x4_t __builtin_aarch64_addhn2v2di (__Int32x2_t, __Int64x2_t, __Int64x2_t);
__Int32x4_t __builtin_aarch64_raddhn2v2di (__Int32x2_t, __Int64x2_t, __Int64x2_t);
__Uint32x4_t __builtin_aarch64_raddhn2v2di (__Int32x2_t, __Int64x2_t, __Int64x2_t);
__Int32x4_t __builtin_aarch64_ssublv4hi (__Int16x4_t, __Int16x4_t);
__Int64x2_t __builtin_aarch64_ssublv2si (__Int32x2_t, __Int32x2_t);
__Uint32x4_t __builtin_aarch64_usublv4hi (__Int16x4_t, __Int16x4_t);
__Uint64x2_t __builtin_aarch64_usublv2si (__Int32x2_t, __Int32x2_t);
__Int16x8_t __builtin_aarch64_ssubl2v16qi (__Int8x16_t, __Int8x16_t);
__Int32x4_t __builtin_aarch64_ssubl2v8hi (__Int16x8_t, __Int16x8_t);
__Int64x2_t __builtin_aarch64_ssubl2v4si (__Int32x4_t, __Int32x4_t);
__Uint16x8_t __builtin_aarch64_usubl2v16qi (__Int8x16_t, __Int8x16_t);
__Uint32x4_t __builtin_aarch64_usubl2v8hi (__Int16x8_t, __Int16x8_t);
__Uint64x2_t __builtin_aarch64_usubl2v4si (__Int32x4_t, __Int32x4_t);
__Int16x8_t __builtin_aarch64_ssubwv8qi (__Int16x8_t, __Int8x8_t);
__Int32x4_t __builtin_aarch64_ssubwv4hi (__Int32x4_t, __Int16x4_t);
__Int64x2_t __builtin_aarch64_ssubwv2si (__Int64x2_t, __Int32x2_t);
__Uint16x8_t __builtin_aarch64_usubwv8qi (__Int16x8_t, __Int8x8_t);
__Uint32x4_t __builtin_aarch64_usubwv4hi (__Int32x4_t, __Int16x4_t);
__Uint64x2_t __builtin_aarch64_usubwv2si (__Int64x2_t, __Int32x2_t);
__Int16x8_t __builtin_aarch64_ssubw2v16qi (__Int16x8_t, __Int8x16_t);
__Int32x4_t __builtin_aarch64_ssubw2v8hi (__Int32x4_t, __Int16x8_t);
__Int64x2_t __builtin_aarch64_ssubw2v4si (__Int64x2_t, __Int32x4_t);
__Uint16x8_t __builtin_aarch64_usubw2v16qi (__Int16x8_t, __Int8x16_t);
__Uint32x4_t __builtin_aarch64_usubw2v8hi (__Int32x4_t, __Int16x8_t);
__Uint64x2_t __builtin_aarch64_usubw2v4si (__Int64x2_t, __Int32x4_t);
__Int8x8_t __builtin_aarch64_sqaddv8qi (__Int8x8_t, __Int8x8_t);
__Int16x4_t __builtin_aarch64_sqaddv4hi (__Int16x4_t, __Int16x4_t);
__Int32x2_t __builtin_aarch64_sqaddv2si (__Int32x2_t, __Int32x2_t);
__Int8x8_t __builtin_aarch64_shsubv8qi (__Int8x8_t, __Int8x8_t);
__Int16x4_t __builtin_aarch64_shsubv4hi (__Int16x4_t, __Int16x4_t);
__Int32x2_t __builtin_aarch64_shsubv2si (__Int32x2_t, __Int32x2_t);
__Uint8x8_t __builtin_aarch64_uhsubv8qi (__Int8x8_t, __Int8x8_t);
__Uint16x4_t __builtin_aarch64_uhsubv4hi (__Int16x4_t, __Int16x4_t);
__Uint32x2_t __builtin_aarch64_uhsubv2si (__Int32x2_t, __Int32x2_t);
__Int8x16_t __builtin_aarch64_shsubv16qi (__Int8x16_t, __Int8x16_t);
__Int16x8_t __builtin_aarch64_shsubv8hi (__Int16x8_t, __Int16x8_t);
__Int32x4_t __builtin_aarch64_shsubv4si (__Int32x4_t, __Int32x4_t);
__Uint8x16_t __builtin_aarch64_uhsubv16qi (__Int8x16_t, __Int8x16_t);
__Uint16x8_t __builtin_aarch64_uhsubv8hi (__Int16x8_t, __Int16x8_t);
__Uint32x4_t __builtin_aarch64_uhsubv4si (__Int32x4_t, __Int32x4_t);
__Int8x8_t __builtin_aarch64_subhnv8hi (__Int16x8_t, __Int16x8_t);
__Int16x4_t __builtin_aarch64_subhnv4si (__Int32x4_t, __Int32x4_t);
__Int32x2_t __builtin_aarch64_subhnv2di (__Int64x2_t, __Int64x2_t);
__Uint8x8_t __builtin_aarch64_subhnv8hi (__Int16x8_t, __Int16x8_t);
__Uint16x4_t __builtin_aarch64_subhnv4si (__Int32x4_t, __Int32x4_t);
__Uint32x2_t __builtin_aarch64_subhnv2di (__Int64x2_t, __Int64x2_t);
__Int8x8_t __builtin_aarch64_rsubhnv8hi (__Int16x8_t, __Int16x8_t);
__Int16x4_t __builtin_aarch64_rsubhnv4si (__Int32x4_t, __Int32x4_t);
__Int32x2_t __builtin_aarch64_rsubhnv2di (__Int64x2_t, __Int64x2_t);
__Uint8x8_t __builtin_aarch64_rsubhnv8hi (__Int16x8_t, __Int16x8_t);
__Uint16x4_t __builtin_aarch64_rsubhnv4si (__Int32x4_t, __Int32x4_t);
__Uint32x2_t __builtin_aarch64_rsubhnv2di (__Int64x2_t, __Int64x2_t);
__Int8x16_t __builtin_aarch64_rsubhn2v8hi (__Int8x8_t, __Int16x8_t, __Int16x8_t);
__Int16x8_t __builtin_aarch64_rsubhn2v4si (__Int16x4_t, __Int32x4_t, __Int32x4_t);
__Int32x4_t __builtin_aarch64_rsubhn2v2di (__Int32x2_t, __Int64x2_t, __Int64x2_t);
__Uint8x16_t __builtin_aarch64_rsubhn2v8hi (__Int8x8_t, __Int16x8_t, __Int16x8_t);
__Uint16x8_t __builtin_aarch64_rsubhn2v4si (__Int16x4_t, __Int32x4_t, __Int32x4_t);
__Uint32x4_t __builtin_aarch64_rsubhn2v2di (__Int32x2_t, __Int64x2_t, __Int64x2_t);
__Int8x16_t __builtin_aarch64_subhn2v8hi (__Int8x8_t, __Int16x8_t, __Int16x8_t);
__Int16x8_t __builtin_aarch64_subhn2v4si (__Int16x4_t, __Int32x4_t, __Int32x4_t);
__Int32x4_t __builtin_aarch64_subhn2v2di (__Int32x2_t, __Int64x2_t, __Int64x2_t);
__Uint8x16_t __builtin_aarch64_subhn2v8hi (__Int8x8_t, __Int16x8_t, __Int16x8_t);
__Uint16x8_t __builtin_aarch64_subhn2v4si (__Int16x4_t, __Int32x4_t, __Int32x4_t);
__Uint32x4_t __builtin_aarch64_subhn2v2di (__Int32x2_t, __Int64x2_t, __Int64x2_t);
__Int8x16_t __builtin_aarch64_sqaddv16qi (__Int8x16_t, __Int8x16_t);
__Int16x8_t __builtin_aarch64_sqaddv8hi (__Int16x8_t, __Int16x8_t);
__Int32x4_t __builtin_aarch64_sqaddv4si (__Int32x4_t, __Int32x4_t);
__Int64x2_t __builtin_aarch64_sqaddv2di (__Int64x2_t, __Int64x2_t);
__Int8x8_t __builtin_aarch64_sqsubv8qi (__Int8x8_t, __Int8x8_t);
__Int16x4_t __builtin_aarch64_sqsubv4hi (__Int16x4_t, __Int16x4_t);
__Int32x2_t __builtin_aarch64_sqsubv2si (__Int32x2_t, __Int32x2_t);
__Int8x16_t __builtin_aarch64_sqsubv16qi (__Int8x16_t, __Int8x16_t);
__Int16x8_t __builtin_aarch64_sqsubv8hi (__Int16x8_t, __Int16x8_t);
__Int32x4_t __builtin_aarch64_sqsubv4si (__Int32x4_t, __Int32x4_t);
__Int64x2_t __builtin_aarch64_sqsubv2di (__Int64x2_t, __Int64x2_t);
__Int8x8_t __builtin_aarch64_sqnegv8qi (__Int8x8_t);
__Int16x4_t __builtin_aarch64_sqnegv4hi (__Int16x4_t);
__Int32x2_t __builtin_aarch64_sqnegv2si (__Int32x2_t);
__Int8x16_t __builtin_aarch64_sqnegv16qi (__Int8x16_t);
__Int16x8_t __builtin_aarch64_sqnegv8hi (__Int16x8_t);
__Int32x4_t __builtin_aarch64_sqnegv4si (__Int32x4_t);
__Int8x8_t __builtin_aarch64_sqabsv8qi (__Int8x8_t);
__Int16x4_t __builtin_aarch64_sqabsv4hi (__Int16x4_t);
__Int32x2_t __builtin_aarch64_sqabsv2si (__Int32x2_t);
__Int8x16_t __builtin_aarch64_sqabsv16qi (__Int8x16_t);
__Int16x8_t __builtin_aarch64_sqabsv8hi (__Int16x8_t);
__Int32x4_t __builtin_aarch64_sqabsv4si (__Int32x4_t);
__Int16x4_t __builtin_aarch64_sqdmulhv4hi (__Int16x4_t, __Int16x4_t);
__Int32x2_t __builtin_aarch64_sqdmulhv2si (__Int32x2_t, __Int32x2_t);
__Int16x8_t __builtin_aarch64_sqdmulhv8hi (__Int16x8_t, __Int16x8_t);
__Int32x4_t __builtin_aarch64_sqdmulhv4si (__Int32x4_t, __Int32x4_t);
__Int16x4_t __builtin_aarch64_sqrdmulhv4hi (__Int16x4_t, __Int16x4_t);
__Int32x2_t __builtin_aarch64_sqrdmulhv2si (__Int32x2_t, __Int32x2_t);
__Int16x8_t __builtin_aarch64_sqrdmulhv8hi (__Int16x8_t, __Int16x8_t);
__Int32x4_t __builtin_aarch64_sqrdmulhv4si (__Int32x4_t, __Int32x4_t);
__Int8x16_t __builtin_aarch64_combinev8qi (__Int8x8_t, __Int8x8_t);
__Int16x8_t __builtin_aarch64_combinev4hi (__Int16x4_t, __Int16x4_t);
__Int32x4_t __builtin_aarch64_combinev2si (__Int32x2_t, __Int32x2_t);
__Float32x4_t __builtin_aarch64_combinev2sf (__Float32x2_t, __Float32x2_t);
__Uint8x16_t __builtin_aarch64_combinev8qi (__Int8x8_t, __Int8x8_t);
__Uint16x8_t __builtin_aarch64_combinev4hi (__Int16x4_t, __Int16x4_t);
__Uint32x4_t __builtin_aarch64_combinev2si (__Int32x2_t, __Int32x2_t);
__Poly8x16_t __builtin_aarch64_combinev8qi (__Int8x8_t, __Int8x8_t);
__Poly16x8_t __builtin_aarch64_combinev4hi (__Int16x4_t, __Int16x4_t);
__Uint8x8_t __builtin_aarch64_clzv8qi (__Int8x8_t);
__Uint16x4_t __builtin_aarch64_clzv4hi (__Int16x4_t);
__Uint32x2_t __builtin_aarch64_clzv2si (__Int32x2_t);
__Uint8x16_t __builtin_aarch64_clzv16qi (__Int8x16_t);
__Uint16x8_t __builtin_aarch64_clzv8hi (__Int16x8_t);
__Uint32x4_t __builtin_aarch64_clzv4si (__Int32x4_t);
__Poly8x8_t __builtin_aarch64_popcountv8qi (__Int8x8_t);
__Uint8x8_t __builtin_aarch64_popcountv8qi (__Int8x8_t);
__Poly8x16_t __builtin_aarch64_popcountv16qi (__Int8x16_t);
__Uint8x16_t __builtin_aarch64_popcountv16qi (__Int8x16_t);
__Uint8x8_t __builtin_aarch64_umaxv8qi (__Int8x8_t, __Int8x8_t);
__Uint16x4_t __builtin_aarch64_umaxv4hi (__Int16x4_t, __Int16x4_t);
__Uint32x2_t __builtin_aarch64_umaxv2si (__Int32x2_t, __Int32x2_t);
__Uint8x16_t __builtin_aarch64_umaxv16qi (__Int8x16_t, __Int8x16_t);
__Uint16x8_t __builtin_aarch64_umaxv8hi (__Int16x8_t, __Int16x8_t);
__Uint32x4_t __builtin_aarch64_umaxv4si (__Int32x4_t, __Int32x4_t);
__Uint8x8_t __builtin_aarch64_umaxpv8qi (__Int8x8_t, __Int8x8_t);
__Uint16x4_t __builtin_aarch64_umaxpv4hi (__Int16x4_t, __Int16x4_t);
__Uint32x2_t __builtin_aarch64_umaxpv2si (__Int32x2_t, __Int32x2_t);
__Uint8x16_t __builtin_aarch64_umaxpv16qi (__Int8x16_t, __Int8x16_t);
__Uint16x8_t __builtin_aarch64_umaxpv8hi (__Int16x8_t, __Int16x8_t);
__Uint32x4_t __builtin_aarch64_umaxpv4si (__Int32x4_t, __Int32x4_t);
__Uint8x8_t __builtin_aarch64_uminpv8qi (__Int8x8_t, __Int8x8_t);
__Uint16x4_t __builtin_aarch64_uminpv4hi (__Int16x4_t, __Int16x4_t);
__Uint32x2_t __builtin_aarch64_uminpv2si (__Int32x2_t, __Int32x2_t);
__Uint8x16_t __builtin_aarch64_uminpv16qi (__Int8x16_t, __Int8x16_t);
__Uint16x8_t __builtin_aarch64_uminpv8hi (__Int16x8_t, __Int16x8_t);
__Uint32x4_t __builtin_aarch64_uminpv4si (__Int32x4_t, __Int32x4_t);
__Uint8x8_t __builtin_aarch64_uminv8qi (__Int8x8_t, __Int8x8_t);
__Uint16x4_t __builtin_aarch64_uminv4hi (__Int16x4_t, __Int16x4_t);
__Uint32x2_t __builtin_aarch64_uminv2si (__Int32x2_t, __Int32x2_t);
__Uint8x16_t __builtin_aarch64_uminv16qi (__Int8x16_t, __Int8x16_t);
__Uint16x8_t __builtin_aarch64_uminv8hi (__Int16x8_t, __Int16x8_t);
__Uint32x4_t __builtin_aarch64_uminv4si (__Int32x4_t, __Int32x4_t);
__Uint8x8_t __builtin_aarch64_addpv8qi (__Int8x8_t, __Int8x8_t);
__Uint16x4_t __builtin_aarch64_addpv4hi (__Int16x4_t, __Int16x4_t);
__Uint32x2_t __builtin_aarch64_addpv2si (__Int32x2_t, __Int32x2_t);
__Int64x2_t __builtin_aarch64_sqabsv2di (__Int64x2_t);
__Int8x8_t __builtin_aarch64_sqmovnv8hi (__Int16x8_t);
__Int16x4_t __builtin_aarch64_sqmovnv4si (__Int32x4_t);
__Int32x2_t __builtin_aarch64_sqmovnv2di (__Int64x2_t);
__Uint8x8_t __builtin_aarch64_uqmovnv8hi (__Int16x8_t);
__Uint16x4_t __builtin_aarch64_uqmovnv4si (__Int32x4_t);
__Uint32x2_t __builtin_aarch64_uqmovnv2di (__Int64x2_t);
__Uint8x8_t __builtin_aarch64_sqmovunv8hi (__Int16x8_t);
__Uint16x4_t __builtin_aarch64_sqmovunv4si (__Int32x4_t);
__Uint32x2_t __builtin_aarch64_sqmovunv2di (__Int64x2_t);
__Int64x2_t __builtin_aarch64_sqnegv2di (__Int64x2_t);
__Int8x8_t __builtin_aarch64_sqrshrn_nv8hi (__Int16x8_t, const int);
__Int16x4_t __builtin_aarch64_sqrshrn_nv4si (__Int32x4_t, const int);
__Int32x2_t __builtin_aarch64_sqrshrn_nv2di (__Int64x2_t, const int);
__Uint8x8_t __builtin_aarch64_sqrshrun_nv8hi (__Int16x8_t, const int);
__Uint16x4_t __builtin_aarch64_sqrshrun_nv4si (__Int32x4_t, const int);
__Uint32x2_t __builtin_aarch64_sqrshrun_nv2di (__Int64x2_t, const int);
__Int8x8_t __builtin_aarch64_sqshl_nv8qi (__Int8x8_t, const int);
__Int16x4_t __builtin_aarch64_sqshl_nv4hi (__Int16x4_t, const int);
__Int32x2_t __builtin_aarch64_sqshl_nv2si (__Int32x2_t, const int);
__Int8x16_t __builtin_aarch64_sqshl_nv16qi (__Int8x16_t, const int);
__Int16x8_t __builtin_aarch64_sqshl_nv8hi (__Int16x8_t, const int);
__Int32x4_t __builtin_aarch64_sqshl_nv4si (__Int32x4_t, const int);
__Int64x2_t __builtin_aarch64_sqshl_nv2di (__Int64x2_t, const int);
__Int8x8_t __builtin_aarch64_sqshrn_nv8hi (__Int16x8_t, const int);
__Int16x4_t __builtin_aarch64_sqshrn_nv4si (__Int32x4_t, const int);
__Int32x2_t __builtin_aarch64_sqshrn_nv2di (__Int64x2_t, const int);
__Uint8x8_t __builtin_aarch64_sqshrun_nv8hi (__Int16x8_t, const int);
__Uint16x4_t __builtin_aarch64_sqshrun_nv4si (__Int32x4_t, const int);
__Uint32x2_t __builtin_aarch64_sqshrun_nv2di (__Int64x2_t, const int);
__Poly8x8_t __builtin_aarch64_rbitv8qi (__Int8x8_t);
__Uint8x8_t __builtin_aarch64_rbitv8qi (__Int8x8_t);
__Poly8x16_t __builtin_aarch64_rbitv16qi (__Int8x16_t);
__Uint8x16_t __builtin_aarch64_rbitv16qi (__Int8x16_t);
__Uint32x2_t __builtin_aarch64_urecpev2si (__Int32x2_t);
__Uint32x4_t __builtin_aarch64_urecpev4si (__Int32x4_t);
__Int8x8_t __builtin_aarch64_srshlv8qi (__Int8x8_t, __Int8x8_t);
__Int16x4_t __builtin_aarch64_srshlv4hi (__Int16x4_t, __Int16x4_t);
__Int32x2_t __builtin_aarch64_srshlv2si (__Int32x2_t, __Int32x2_t);
__Int8x16_t __builtin_aarch64_srshlv16qi (__Int8x16_t, __Int8x16_t);
__Int16x8_t __builtin_aarch64_srshlv8hi (__Int16x8_t, __Int16x8_t);
__Int32x4_t __builtin_aarch64_srshlv4si (__Int32x4_t, __Int32x4_t);
__Int64x2_t __builtin_aarch64_srshlv2di (__Int64x2_t, __Int64x2_t);
__Int8x8_t __builtin_aarch64_srshr_nv8qi (__Int8x8_t, const int);
__Int16x4_t __builtin_aarch64_srshr_nv4hi (__Int16x4_t, const int);
__Int32x2_t __builtin_aarch64_srshr_nv2si (__Int32x2_t, const int);
__Int8x16_t __builtin_aarch64_srshr_nv16qi (__Int8x16_t, const int);
__Int16x8_t __builtin_aarch64_srshr_nv8hi (__Int16x8_t, const int);
__Int32x4_t __builtin_aarch64_srshr_nv4si (__Int32x4_t, const int);
__Int64x2_t __builtin_aarch64_srshr_nv2di (__Int64x2_t, const int);
__Int8x8_t __builtin_aarch64_srsra_nv8qi (__Int8x8_t, __Int8x8_t, const int);
__Int16x4_t __builtin_aarch64_srsra_nv4hi (__Int16x4_t, __Int16x4_t, const int);
__Int32x2_t __builtin_aarch64_srsra_nv2si (__Int32x2_t, __Int32x2_t, const int);
__Int8x16_t __builtin_aarch64_srsra_nv16qi (__Int8x16_t, __Int8x16_t, const int);
__Int16x8_t __builtin_aarch64_srsra_nv8hi (__Int16x8_t, __Int16x8_t, const int);
__Int32x4_t __builtin_aarch64_srsra_nv4si (__Int32x4_t, __Int32x4_t, const int);
__Int64x2_t __builtin_aarch64_srsra_nv2di (__Int64x2_t, __Int64x2_t, const int);
__Int8x8_t __builtin_aarch64_ashlv8qi (__Int8x8_t, const int);
__Int16x4_t __builtin_aarch64_ashlv4hi (__Int16x4_t, const int);
__Int32x2_t __builtin_aarch64_ashlv2si (__Int32x2_t, const int);
__Uint8x8_t __builtin_aarch64_ashlv8qi (__Int8x8_t, const int);
__Uint16x4_t __builtin_aarch64_ashlv4hi (__Int16x4_t, const int);
__Uint32x2_t __builtin_aarch64_ashlv2si (__Int32x2_t, const int);
__Int8x16_t __builtin_aarch64_ashlv16qi (__Int8x16_t, const int);
__Int16x8_t __builtin_aarch64_ashlv8hi (__Int16x8_t, const int);
__Int32x4_t __builtin_aarch64_ashlv4si (__Int32x4_t, const int);
__Int64x2_t __builtin_aarch64_ashlv2di (__Int64x2_t, const int);
__Uint8x16_t __builtin_aarch64_ashlv16qi (__Int8x16_t, const int);
__Uint16x8_t __builtin_aarch64_ashlv8hi (__Int16x8_t, const int);
__Uint32x4_t __builtin_aarch64_ashlv4si (__Int32x4_t, const int);
__Uint64x2_t __builtin_aarch64_ashlv2di (__Int64x2_t, const int);
__Uint16x8_t __builtin_aarch64_ushll2_nv16qi (__Int8x16_t, const int);
__Uint32x4_t __builtin_aarch64_ushll2_nv8hi (__Int16x8_t, const int);
__Uint64x2_t __builtin_aarch64_ushll2_nv4si (__Int32x4_t, const int);
__Int8x8_t __builtin_aarch64_ashrv8qi (__Int8x8_t, const int);
__Int16x4_t __builtin_aarch64_ashrv4hi (__Int16x4_t, const int);
__Int32x2_t __builtin_aarch64_ashrv2si (__Int32x2_t, const int);
__Uint8x8_t __builtin_aarch64_lshrv8qi (__Int8x8_t, const int);
__Uint16x4_t __builtin_aarch64_lshrv4hi (__Int16x4_t, const int);
__Uint32x2_t __builtin_aarch64_lshrv2si (__Int32x2_t, const int);
__Int8x16_t __builtin_aarch64_ashrv16qi (__Int8x16_t, const int);
__Int16x8_t __builtin_aarch64_ashrv8hi (__Int16x8_t, const int);
__Int32x4_t __builtin_aarch64_ashrv4si (__Int32x4_t, const int);
__Int64x2_t __builtin_aarch64_ashrv2di (__Int64x2_t, const int);
__Uint8x16_t __builtin_aarch64_lshrv16qi (__Int8x16_t, const int);
__Uint16x8_t __builtin_aarch64_lshrv8hi (__Int16x8_t, const int);
__Int8x8_t __builtin_aarch64_ssli_nv8qi (__Int8x8_t, __Int8x8_t, const int);
__Int16x4_t __builtin_aarch64_ssli_nv4hi (__Int16x4_t, __Int16x4_t, const int);
__Int32x2_t __builtin_aarch64_ssli_nv2si (__Int32x2_t, __Int32x2_t, const int);
__Int8x16_t __builtin_aarch64_ssli_nv16qi (__Int8x16_t, __Int8x16_t, const int);
__Int16x8_t __builtin_aarch64_ssli_nv8hi (__Int16x8_t, __Int16x8_t, const int);
__Int32x4_t __builtin_aarch64_ssli_nv4si (__Int32x4_t, __Int32x4_t, const int);
__Int64x2_t __builtin_aarch64_ssli_nv2di (__Int64x2_t, __Int64x2_t, const int);
__Int8x8_t __builtin_aarch64_ssra_nv8qi (__Int8x8_t, __Int8x8_t, const int);
__Int16x4_t __builtin_aarch64_ssra_nv4hi (__Int16x4_t, __Int16x4_t, const int);
__Int32x2_t __builtin_aarch64_ssra_nv2si (__Int32x2_t, __Int32x2_t, const int);
__Int8x16_t __builtin_aarch64_ssra_nv16qi (__Int8x16_t, __Int8x16_t, const int);
__Int16x8_t __builtin_aarch64_ssra_nv8hi (__Int16x8_t, __Int16x8_t, const int);
__Int32x4_t __builtin_aarch64_ssra_nv4si (__Int32x4_t, __Int32x4_t, const int);
__Int64x2_t __builtin_aarch64_ssra_nv2di (__Int64x2_t, __Int64x2_t, const int);
__Int8x8_t __builtin_aarch64_ssri_nv8qi (__Int8x8_t, __Int8x8_t, const int);
__Int16x4_t __builtin_aarch64_ssri_nv4hi (__Int16x4_t, __Int16x4_t, const int);
__Int32x2_t __builtin_aarch64_ssri_nv2si (__Int32x2_t, __Int32x2_t, const int);
__Int8x16_t __builtin_aarch64_ssri_nv16qi (__Int8x16_t, __Int8x16_t, const int);
__Int16x8_t __builtin_aarch64_ssri_nv8hi (__Int16x8_t, __Int16x8_t, const int);
__Int32x4_t __builtin_aarch64_ssri_nv4si (__Int32x4_t, __Int32x4_t, const int);
__Int64x2_t __builtin_aarch64_ssri_nv2di (__Int64x2_t, __Int64x2_t, const int);
__Uint8x8_t __builtin_aarch64_uqaddv8qi_uuu (__Uint8x8_t, __Uint8x8_t);
__Uint32x2_t __builtin_aarch64_uqaddv2si_uuu (__Uint32x2_t, __Uint32x2_t);
__Uint8x16_t __builtin_aarch64_uqaddv16qi_uuu (__Uint8x16_t, __Uint8x16_t);
__Uint16x8_t __builtin_aarch64_uqaddv8hi_uuu (__Uint16x8_t, __Uint16x8_t);
__Uint32x4_t __builtin_aarch64_uqaddv4si_uuu (__Uint32x4_t, __Uint32x4_t);
__Uint64x2_t __builtin_aarch64_uqaddv2di_uuu (__Uint64x2_t, __Uint64x2_t);
__Uint8x8_t __builtin_aarch64_uqsubv8qi_uuu (__Uint8x8_t, __Uint8x8_t);
__Uint16x4_t __builtin_aarch64_uqsubv4hi_uuu (__Uint16x4_t, __Uint16x4_t);
__Uint32x2_t __builtin_aarch64_uqsubv2si_uuu (__Uint32x2_t, __Uint32x2_t);
__Uint8x16_t __builtin_aarch64_uqsubv16qi_uuu (__Uint8x16_t, __Uint8x16_t);
__Uint16x8_t __builtin_aarch64_uqsubv8hi_uuu (__Uint16x8_t, __Uint16x8_t);
__Uint32x4_t __builtin_aarch64_uqsubv4si_uuu (__Uint32x4_t, __Uint32x4_t);
__Uint64x2_t __builtin_aarch64_uqsubv2di_uuu (__Uint64x2_t, __Uint64x2_t);
__Float16x8_t __builtin_aarch64_combinev4hf (__Float16x4_t, __Float16x4_t);
__Uint16x4_t __builtin_aarch64_uqaddv4hi_uuu(__Uint16x4_t, __Uint16x4_t);
__Int16x4_t __builtin_aarch64_sqdmulh_laneqv4hi(__Int16x4_t, __Int16x8_t, const int);
__Int32x2_t __builtin_aarch64_sqdmulh_laneqv2si(__Int32x2_t, __Int32x4_t, const int);
__Int16x8_t __builtin_aarch64_sqdmulh_laneqv8hi(__Int16x8_t, __Int16x8_t, const int);
__Int32x4_t __builtin_aarch64_sqdmulh_laneqv4si(__Int32x4_t, __Int32x4_t, const int);
__Int16x4_t __builtin_aarch64_sqrdmulh_laneqv4hi(__Int16x4_t, __Int16x8_t, const int);
__Int32x2_t __builtin_aarch64_sqrdmulh_laneqv2si(__Int32x2_t, __Int32x4_t, const int);
__Int16x8_t __builtin_aarch64_sqrdmulh_laneqv8hi(__Int16x8_t, __Int16x8_t, const int);
__Int32x4_t __builtin_aarch64_sqrdmulh_laneqv4si(__Int32x4_t, __Int32x4_t, const int);
__Uint16x4_t __builtin_aarch64_uqaddv4hi_uuu (__Uint16x4_t __a, __Uint16x4_t __b);
__Int64x2_t __builtin_aarch64_combinedi (long int, long int);
__Float64x2_t __builtin_aarch64_combinedf (float , float);
__Int16x4_t __builtin_aarch64_sqdmulh_laneqv4hi (__Int16x4_t __a, __Int16x8_t __b, const int __c);
__Int32x2_t __builtin_aarch64_sqdmulh_laneqv2si (__Int32x2_t __a, __Int32x4_t __b, const int __c);
__Int16x8_t __builtin_aarch64_sqdmulh_laneqv8hi (__Int16x8_t __a, __Int16x8_t __b, const int __c);
__Int32x4_t __builtin_aarch64_sqdmulh_laneqv4si (__Int32x4_t __a, __Int32x4_t __b, const int __c);
__Int16x4_t __builtin_aarch64_sqrdmulh_laneqv4hi (__Int16x4_t __a, __Int16x8_t __b, const int __c);
__Int32x2_t __builtin_aarch64_sqrdmulh_laneqv2si (__Int32x2_t __a, __Int32x4_t __b, const int __c);
__Int16x8_t __builtin_aarch64_sqrdmulh_laneqv8hi (__Int16x8_t __a, __Int16x8_t __b, const int __c);
__Int32x4_t __builtin_aarch64_sqrdmulh_laneqv4si (__Int32x4_t __a, __Int32x4_t __b, const int __c);
__Int8x8_t __builtin_aarch64_absv8qi (__Int8x8_t __a);
__Int16x4_t __builtin_aarch64_absv4hi (__Int16x4_t __a);
__Int32x2_t __builtin_aarch64_absv2si (__Int32x2_t __a);
__Float32x4_t __builtin_aarch64_absv4sf (__Float32x4_t __a);
__Float64x2_t __builtin_aarch64_absv2df (__Float64x2_t __a);
__Int8x16_t __builtin_aarch64_absv16qi (__Int8x16_t __a);
__Int16x8_t __builtin_aarch64_absv8hi (__Int16x8_t __a);
__Int32x4_t __builtin_aarch64_absv4si (__Int32x4_t __a);
__Int64x2_t __builtin_aarch64_absv2di (__Int64x2_t __a);
__Float32x2_t __builtin_aarch64_simd_bslv2sf_suss (__Uint32x2_t __a, __Float32x2_t __b, __Float32x2_t __c);
__Poly8x8_t __builtin_aarch64_simd_bslv8qi_pupp (__Uint8x8_t __a, __Poly8x8_t __b, __Poly8x8_t __c);
__Poly16x4_t __builtin_aarch64_simd_bslv4hi_pupp (__Uint16x4_t __a, __Poly16x4_t __b, __Poly16x4_t __c);
__Int8x8_t __builtin_aarch64_simd_bslv8qi_suss (__Uint8x8_t __a, __Int8x8_t __b, __Int8x8_t __c);
__Int16x4_t __builtin_aarch64_simd_bslv4hi_suss (__Uint16x4_t __a, __Int16x4_t __b, __Int16x4_t __c);
__Int32x2_t __builtin_aarch64_simd_bslv2si_suss (__Uint32x2_t __a, __Int32x2_t __b, __Int32x2_t __c);
__Uint8x8_t __builtin_aarch64_simd_bslv8qi_uuuu (__Uint8x8_t __a, __Uint8x8_t __b, __Uint8x8_t __c);
__Uint16x4_t __builtin_aarch64_simd_bslv4hi_uuuu (__Uint16x4_t __a, __Uint16x4_t __b, __Uint16x4_t __c);
__Uint32x2_t __builtin_aarch64_simd_bslv2si_uuuu (__Uint32x2_t __a, __Uint32x2_t __b, __Uint32x2_t __c);
__Float32x4_t __builtin_aarch64_simd_bslv4sf_suss (__Uint32x4_t __a, __Float32x4_t __b, __Float32x4_t __c);
__Float64x2_t __builtin_aarch64_simd_bslv2df_suss (__Uint64x2_t __a, __Float64x2_t __b, __Float64x2_t __c);
__Poly8x16_t __builtin_aarch64_simd_bslv16qi_pupp (__Uint8x16_t __a, __Poly8x16_t __b, __Poly8x16_t __c);
__Poly16x8_t __builtin_aarch64_simd_bslv8hi_pupp (__Uint16x8_t __a, __Poly16x8_t __b, __Poly16x8_t __c);
__Int8x16_t __builtin_aarch64_simd_bslv16qi_suss (__Uint8x16_t __a, __Int8x16_t __b, __Int8x16_t __c);
__Int16x8_t __builtin_aarch64_simd_bslv8hi_suss (__Uint16x8_t __a, __Int16x8_t __b, __Int16x8_t __c);
__Int32x4_t __builtin_aarch64_simd_bslv4si_suss (__Uint32x4_t __a, __Int32x4_t __b, __Int32x4_t __c);
__Int64x2_t __builtin_aarch64_simd_bslv2di_suss (__Uint64x2_t __a, __Int64x2_t __b, __Int64x2_t __c);
__Uint8x16_t __builtin_aarch64_simd_bslv16qi_uuuu (__Uint8x16_t __a, __Uint8x16_t __b, __Uint8x16_t __c);
__Uint16x8_t __builtin_aarch64_simd_bslv8hi_uuuu (__Uint16x8_t __a, __Uint16x8_t __b, __Uint16x8_t __c);
__Uint32x4_t __builtin_aarch64_simd_bslv4si_uuuu (__Uint32x4_t __a, __Uint32x4_t __b, __Uint32x4_t __c);
__Uint64x2_t __builtin_aarch64_simd_bslv2di_uuuu (__Uint64x2_t __a, __Uint64x2_t __b, __Uint64x2_t __c);
__Int16x4_t __builtin_aarch64_sqrdmlahv4hi (__Int16x4_t __a, __Int16x4_t __b, __Int16x4_t __c);
__Int32x2_t __builtin_aarch64_sqrdmlahv2si (__Int32x2_t __a, __Int32x2_t __b, __Int32x2_t __c);
__Int16x8_t __builtin_aarch64_sqrdmlahv8hi (__Int16x8_t __a, __Int16x8_t __b, __Int16x8_t __c);
__Int32x4_t __builtin_aarch64_sqrdmlahv4si (__Int32x4_t __a, __Int32x4_t __b, __Int32x4_t __c);
__Int16x4_t __builtin_aarch64_sqrdmlshv4hi (__Int16x4_t __a, __Int16x4_t __b, __Int16x4_t __c);
__Int32x2_t __builtin_aarch64_sqrdmlshv2si (__Int32x2_t __a, __Int32x2_t __b, __Int32x2_t __c);
__Int16x8_t __builtin_aarch64_sqrdmlshv8hi (__Int16x8_t __a, __Int16x8_t __b, __Int16x8_t __c);
__Int32x4_t __builtin_aarch64_sqrdmlshv4si (__Int32x4_t __a, __Int32x4_t __b, __Int32x4_t __c);
__Int16x4_t __builtin_aarch64_sqrdmlah_laneqv4hi (__Int16x4_t __a, __Int16x4_t __b, __Int16x8_t __c, const int __d);
__Int32x2_t __builtin_aarch64_sqrdmlah_laneqv2si (__Int32x2_t __a, __Int32x2_t __b, __Int32x4_t __c, const int __d);
__Int16x8_t __builtin_aarch64_sqrdmlah_laneqv8hi (__Int16x8_t __a, __Int16x8_t __b, __Int16x8_t __c, const int __d);
__Int32x4_t __builtin_aarch64_sqrdmlah_laneqv4si (__Int32x4_t __a, __Int32x4_t __b, __Int32x4_t __c, const int __d);
__Int16x4_t __builtin_aarch64_sqrdmlsh_laneqv4hi (__Int16x4_t __a, __Int16x4_t __b, __Int16x8_t __c, const int __d);
__Int32x2_t __builtin_aarch64_sqrdmlsh_laneqv2si (__Int32x2_t __a, __Int32x2_t __b, __Int32x4_t __c, const int __d);
__Int16x8_t __builtin_aarch64_sqrdmlsh_laneqv8hi (__Int16x8_t __a, __Int16x8_t __b, __Int16x8_t __c, const int __d);
__Int32x4_t __builtin_aarch64_sqrdmlsh_laneqv4si (__Int32x4_t __a, __Int32x4_t __b, __Int32x4_t __c, const int __d);
__Int16x4_t __builtin_aarch64_sqrdmlah_lanev4hi (__Int16x4_t __a, __Int16x4_t __b, __Int16x4_t __c, const int __d);
__Int32x2_t __builtin_aarch64_sqrdmlah_lanev2si (__Int32x2_t __a, __Int32x2_t __b, __Int32x2_t __c, const int __d);
__Int16x8_t __builtin_aarch64_sqrdmlah_lanev8hi (__Int16x8_t __a, __Int16x8_t __b, __Int16x4_t __c, const int __d);
__Int32x4_t __builtin_aarch64_sqrdmlah_lanev4si (__Int32x4_t __a, __Int32x4_t __b, __Int32x2_t __c, const int __d);
__Int16x4_t __builtin_aarch64_sqrdmlsh_lanev4hi (__Int16x4_t __a, __Int16x4_t __b, __Int16x4_t __c, const int __d);
__Int32x2_t __builtin_aarch64_sqrdmlsh_lanev2si (__Int32x2_t __a, __Int32x2_t __b, __Int32x2_t __c, const int __d);
__Int16x8_t __builtin_aarch64_sqrdmlsh_lanev8hi (__Int16x8_t __a, __Int16x8_t __b, __Int16x4_t __c, const int __d);
__Int32x4_t __builtin_aarch64_sqrdmlsh_lanev4si (__Int32x4_t __a, __Int32x4_t __b, __Int32x2_t __c, const int __d);
__Uint8x16_t __builtin_aarch64_crypto_aesev16qi_uuu (__Uint8x16_t data, __Uint8x16_t key);
__Uint8x16_t __builtin_aarch64_crypto_aesdv16qi_uuu (__Uint8x16_t data, __Uint8x16_t key);
__Uint8x16_t __builtin_aarch64_crypto_aesmcv16qi_uu (__Uint8x16_t data);
__Uint8x16_t __builtin_aarch64_crypto_aesimcv16qi_uu (__Uint8x16_t data);
__Int8x8_t __builtin_aarch64_clrsbv8qi (__Int8x8_t __a);
__Int16x4_t __builtin_aarch64_clrsbv4hi (__Int16x4_t __a);
__Int32x2_t __builtin_aarch64_clrsbv2si (__Int32x2_t __a);
__Int8x16_t __builtin_aarch64_clrsbv16qi (__Int8x16_t __a);
__Int16x8_t __builtin_aarch64_clrsbv8hi (__Int16x8_t __a);
__Int32x4_t __builtin_aarch64_clrsbv4si (__Int32x4_t __a);
__Int8x8_t __builtin_aarch64_clzv8qi (__Int8x8_t __a);
__Int16x4_t __builtin_aarch64_clzv4hi (__Int16x4_t __a);
__Int32x2_t __builtin_aarch64_clzv2si (__Int32x2_t __a);
__Int8x16_t __builtin_aarch64_clzv16qi (__Int8x16_t __a);
__Int16x8_t __builtin_aarch64_clzv8hi (__Int16x8_t __a);
__Int32x4_t __builtin_aarch64_clzv4si (__Int32x4_t __a);
__Int8x8_t __builtin_aarch64_popcountv8qi (__Int8x8_t __a);
__Int8x16_t __builtin_aarch64_popcountv16qi (__Int8x16_t __a);
__Float16x4_t __builtin_aarch64_float_truncate_lo_v4hf (__Float32x4_t __a);
__Float16x8_t __builtin_aarch64_float_truncate_hi_v8hf (__Float16x4_t __a, __Float32x4_t __b);
__Float32x2_t __builtin_aarch64_float_truncate_lo_v2sf (__Float64x2_t __a);
__Float32x4_t __builtin_aarch64_float_truncate_hi_v4sf (__Float32x2_t __a, __Float64x2_t __b);
__Float32x4_t __builtin_aarch64_float_extend_lo_v4sf (__Float16x4_t __a);
__Float64x2_t __builtin_aarch64_float_extend_lo_v2df (__Float32x2_t __a);
__Float32x4_t __builtin_aarch64_vec_unpacks_hi_v8hf (__Float16x8_t __a);
__Float64x2_t __builtin_aarch64_vec_unpacks_hi_v4sf (__Float32x4_t __a);
__Float32x2_t __builtin_aarch64_floatv2siv2sf (__Int32x2_t __a);
__Float32x2_t __builtin_aarch64_floatunsv2siv2sf (__Int32x2_t __a);
__Float32x4_t __builtin_aarch64_floatv4siv4sf (__Int32x4_t __a);
__Float32x4_t __builtin_aarch64_floatunsv4siv4sf (__Int32x4_t __a);
__Float64x2_t __builtin_aarch64_floatv2div2df (__Int64x2_t __a);
__Float64x2_t __builtin_aarch64_floatunsv2div2df (__Int64x2_t __a);
__Int32x2_t __builtin_aarch64_lbtruncv2sfv2si (__Float32x2_t __a);
__Uint32x2_t __builtin_aarch64_lbtruncuv2sfv2si_us (__Float32x2_t __a);
__Int32x4_t __builtin_aarch64_lbtruncv4sfv4si (__Float32x4_t __a);
__Uint32x4_t __builtin_aarch64_lbtruncuv4sfv4si_us (__Float32x4_t __a);
__Int64x2_t __builtin_aarch64_lbtruncv2dfv2di (__Float64x2_t __a);
__Uint64x2_t __builtin_aarch64_lbtruncuv2dfv2di_us (__Float64x2_t __a);
__Int32x2_t __builtin_aarch64_lroundv2sfv2si (__Float32x2_t __a);
__Uint32x2_t __builtin_aarch64_lrounduv2sfv2si_us (__Float32x2_t __a);
__Int32x4_t __builtin_aarch64_lroundv4sfv4si (__Float32x4_t __a);
__Uint32x4_t __builtin_aarch64_lrounduv4sfv4si_us (__Float32x4_t __a);
__Int64x2_t __builtin_aarch64_lroundv2dfv2di (__Float64x2_t __a);
__Uint64x2_t __builtin_aarch64_lrounduv2dfv2di_us (__Float64x2_t __a);
__Int32x2_t __builtin_aarch64_lfloorv2sfv2si (__Float32x2_t __a);
__Uint32x2_t __builtin_aarch64_lflooruv2sfv2si_us (__Float32x2_t __a);
__Int32x4_t __builtin_aarch64_lfloorv4sfv4si (__Float32x4_t __a);
__Uint32x4_t __builtin_aarch64_lflooruv4sfv4si_us (__Float32x4_t __a);
__Int64x2_t __builtin_aarch64_lfloorv2dfv2di (__Float64x2_t __a);
__Uint64x2_t __builtin_aarch64_lflooruv2dfv2di_us (__Float64x2_t __a);
__Int32x2_t __builtin_aarch64_lfrintnv2sfv2si (__Float32x2_t __a);
__Uint32x2_t __builtin_aarch64_lfrintnuv2sfv2si_us (__Float32x2_t __a);
__Int32x4_t __builtin_aarch64_lfrintnv4sfv4si (__Float32x4_t __a);
__Uint32x4_t __builtin_aarch64_lfrintnuv4sfv4si_us (__Float32x4_t __a);
__Int64x2_t __builtin_aarch64_lfrintnv2dfv2di (__Float64x2_t __a);
__Uint64x2_t __builtin_aarch64_lfrintnuv2dfv2di_us (__Float64x2_t __a);
__Int32x2_t __builtin_aarch64_lceilv2sfv2si (__Float32x2_t __a);
__Uint32x2_t __builtin_aarch64_lceiluv2sfv2si_us (__Float32x2_t __a);
__Int32x4_t __builtin_aarch64_lceilv4sfv4si (__Float32x4_t __a);
__Uint32x4_t __builtin_aarch64_lceiluv4sfv4si_us (__Float32x4_t __a);
__Int64x2_t __builtin_aarch64_lceilv2dfv2di (__Float64x2_t __a);
__Uint64x2_t __builtin_aarch64_lceiluv2dfv2di_us (__Float64x2_t __a);
__Float32x2_t __builtin_aarch64_fmav2sf (__Float32x2_t __a, __Float32x2_t __b, __Float32x2_t __c);
__Float32x4_t __builtin_aarch64_fmav4sf (__Float32x4_t __a, __Float32x4_t __b, __Float32x4_t __c);
__Float64x2_t __builtin_aarch64_fmav2df (__Float64x2_t __a, __Float64x2_t __b, __Float64x2_t __c);
__Float16x4_t __builtin_aarch64_ld1v4hf (const __fp16 *__a);
__Float32x2_t __builtin_aarch64_ld1v2sf (const __builtin_aarch64_simd_sf * a);
__Int8x8_t __builtin_aarch64_ld1v8qi (const __builtin_aarch64_simd_qi * a);
__Int16x4_t __builtin_aarch64_ld1v4hi (const __builtin_aarch64_simd_hi * a);
__Int32x2_t __builtin_aarch64_ld1v2si (const __builtin_aarch64_simd_si * a);
__Float16x8_t __builtin_aarch64_ld1v8hf (const __fp16 *__a);
__Float32x4_t __builtin_aarch64_ld1v4sf (const __builtin_aarch64_simd_sf * a);
__Float64x2_t __builtin_aarch64_ld1v2df (const __builtin_aarch64_simd_df * a);
__Int8x16_t __builtin_aarch64_ld1v16qi (const __builtin_aarch64_simd_qi * a);
__Int16x8_t __builtin_aarch64_ld1v8hi (const __builtin_aarch64_simd_hi * a);
__Int32x4_t __builtin_aarch64_ld1v4si (const __builtin_aarch64_simd_si * a);
__Int64x2_t __builtin_aarch64_ld1v2di (const __builtin_aarch64_simd_di * a);
__Float32x2_t __builtin_aarch64_smax_nanv2sf (__Float32x2_t __a, __Float32x2_t __b);
__Int8x8_t __builtin_aarch64_smaxv8qi (__Int8x8_t __a, __Int8x8_t __b);
__Int16x4_t __builtin_aarch64_smaxv4hi (__Int16x4_t __a, __Int16x4_t __b);
__Int32x2_t __builtin_aarch64_smaxv2si (__Int32x2_t __a, __Int32x2_t __b);
__Float32x4_t __builtin_aarch64_smax_nanv4sf (__Float32x4_t __a, __Float32x4_t __b);
__Float64x2_t __builtin_aarch64_smax_nanv2df (__Float64x2_t __a, __Float64x2_t __b);
__Int8x16_t __builtin_aarch64_smaxv16qi (__Int8x16_t __a, __Int8x16_t __b);
__Int16x8_t __builtin_aarch64_smaxv8hi (__Int16x8_t __a, __Int16x8_t __b);
__Int32x4_t __builtin_aarch64_smaxv4si (__Int32x4_t __a, __Int32x4_t __b);
__Int8x8_t __builtin_aarch64_smaxpv8qi (__Int8x8_t a, __Int8x8_t b);
__Int16x4_t __builtin_aarch64_smaxpv4hi (__Int16x4_t a, __Int16x4_t b);
__Int32x2_t __builtin_aarch64_smaxpv2si (__Int32x2_t a, __Int32x2_t b);
__Int8x16_t __builtin_aarch64_smaxpv16qi (__Int8x16_t a, __Int8x16_t b);
__Int16x8_t __builtin_aarch64_smaxpv8hi (__Int16x8_t a, __Int16x8_t b);
__Int32x4_t __builtin_aarch64_smaxpv4si (__Int32x4_t a, __Int32x4_t b);
__Float32x4_t __builtin_aarch64_smax_nanpv4sf (__Float32x4_t a, __Float32x4_t b);
__Float64x2_t __builtin_aarch64_smax_nanpv2df (__Float64x2_t a, __Float64x2_t b);
__Float32x2_t __builtin_aarch64_smaxpv2sf (__Float32x2_t a, __Float32x2_t b);
__Float32x4_t __builtin_aarch64_smaxpv4sf (__Float32x4_t a, __Float32x4_t b);
__Float64x2_t __builtin_aarch64_smaxpv2df (__Float64x2_t a, __Float64x2_t b);
__Int8x8_t __builtin_aarch64_sminpv8qi (__Int8x8_t a, __Int8x8_t b);
__Int16x4_t __builtin_aarch64_sminpv4hi (__Int16x4_t a, __Int16x4_t b);
__Int32x2_t __builtin_aarch64_sminpv2si (__Int32x2_t a, __Int32x2_t b);
__Int8x16_t __builtin_aarch64_sminpv16qi (__Int8x16_t a, __Int8x16_t b);
__Int16x8_t __builtin_aarch64_sminpv8hi (__Int16x8_t a, __Int16x8_t b);
__Int32x4_t __builtin_aarch64_sminpv4si (__Int32x4_t a, __Int32x4_t b);
__Float32x2_t __builtin_aarch64_smin_nanpv2sf (__Float32x2_t a, __Float32x2_t b);
__Float32x4_t __builtin_aarch64_smin_nanpv4sf (__Float32x4_t a, __Float32x4_t b);
__Float64x2_t __builtin_aarch64_smin_nanpv2df (__Float64x2_t a, __Float64x2_t b);
__Float32x2_t __builtin_aarch64_sminpv2sf (__Float32x2_t a, __Float32x2_t b);
__Float32x4_t __builtin_aarch64_sminpv4sf (__Float32x4_t a, __Float32x4_t b);
__Float64x2_t __builtin_aarch64_sminpv2df (__Float64x2_t a, __Float64x2_t b);
__Float32x2_t __builtin_aarch64_smaxv2sf (__Float32x2_t __a, __Float32x2_t __b);
__Float32x4_t __builtin_aarch64_smaxv4sf (__Float32x4_t __a, __Float32x4_t __b);
__Float64x2_t __builtin_aarch64_smaxv2df (__Float64x2_t __a, __Float64x2_t __b);
__Float32x2_t __builtin_aarch64_smin_nanv2sf (__Float32x2_t __a, __Float32x2_t __b);
__Int8x8_t __builtin_aarch64_sminv8qi (__Int8x8_t __a, __Int8x8_t __b);
__Int16x4_t __builtin_aarch64_sminv4hi (__Int16x4_t __a, __Int16x4_t __b);
__Int32x2_t __builtin_aarch64_sminv2si (__Int32x2_t __a, __Int32x2_t __b);
__Float32x4_t __builtin_aarch64_smin_nanv4sf (__Float32x4_t __a, __Float32x4_t __b);
__Float64x2_t __builtin_aarch64_smin_nanv2df (__Float64x2_t __a, __Float64x2_t __b);
__Int8x16_t __builtin_aarch64_sminv16qi (__Int8x16_t __a, __Int8x16_t __b);
__Int16x8_t __builtin_aarch64_sminv8hi (__Int16x8_t __a, __Int16x8_t __b);
__Int32x4_t __builtin_aarch64_sminv4si (__Int32x4_t __a, __Int32x4_t __b);
__Float32x2_t __builtin_aarch64_sminv2sf (__Float32x2_t __a, __Float32x2_t __b);
__Float32x4_t __builtin_aarch64_sminv4sf (__Float32x4_t __a, __Float32x4_t __b);
__Float64x2_t __builtin_aarch64_sminv2df (__Float64x2_t __a, __Float64x2_t __b);
__Int8x8_t __builtin_aarch64_addpv8qi (__Int8x8_t __a, __Int8x8_t __b);
__Int16x4_t __builtin_aarch64_addpv4hi (__Int16x4_t __a, __Int16x4_t __b);
__Int32x2_t __builtin_aarch64_addpv2si (__Int32x2_t __a, __Int32x2_t __b);
__Int32x4_t __builtin_aarch64_sqdmlalv4hi (__Int32x4_t __a, __Int16x4_t __b, __Int16x4_t __c);
__Int32x4_t __builtin_aarch64_sqdmlal2v8hi (__Int32x4_t __a, __Int16x8_t __b, __Int16x8_t __c);
__Int32x4_t __builtin_aarch64_sqdmlal2_lanev8hi (__Int32x4_t __a, __Int16x8_t __b, __Int16x4_t __c, int const __d);
__Int32x4_t __builtin_aarch64_sqdmlal2_laneqv8hi (__Int32x4_t __a, __Int16x8_t __b, __Int16x8_t __c, int const __d);
__Int32x4_t __builtin_aarch64_sqdmlal2_nv8hi (__Int32x4_t __a, __Int16x8_t __b, short int __c);
__Int32x4_t __builtin_aarch64_sqdmlal_lanev4hi (__Int32x4_t __a, __Int16x4_t __b, __Int16x4_t __c, int const __d);
__Int32x4_t __builtin_aarch64_sqdmlal_laneqv4hi (__Int32x4_t __a, __Int16x4_t __b, __Int16x8_t __c, int const __d);
__Int32x4_t __builtin_aarch64_sqdmlal_nv4hi (__Int32x4_t __a, __Int16x4_t __b, short int __c);
__Int64x2_t __builtin_aarch64_sqdmlalv2si (__Int64x2_t __a, __Int32x2_t __b, __Int32x2_t __c);
__Int64x2_t __builtin_aarch64_sqdmlal2v4si (__Int64x2_t __a, __Int32x4_t __b, __Int32x4_t __c);
__Int64x2_t __builtin_aarch64_sqdmlal2_lanev4si (__Int64x2_t __a, __Int32x4_t __b, __Int32x2_t __c, int const __d);
__Int64x2_t __builtin_aarch64_sqdmlal2_laneqv4si (__Int64x2_t __a, __Int32x4_t __b, __Int32x4_t __c, int const __d);
__Int64x2_t __builtin_aarch64_sqdmlal2_nv4si (__Int64x2_t __a, __Int32x4_t __b, int __c);
__Int64x2_t __builtin_aarch64_sqdmlal_lanev2si (__Int64x2_t __a, __Int32x2_t __b, __Int32x2_t __c, int const __d);
__Int64x2_t __builtin_aarch64_sqdmlal_laneqv2si (__Int64x2_t __a, __Int32x2_t __b, __Int32x4_t __c, int const __d);
__Int64x2_t __builtin_aarch64_sqdmlal_nv2si (__Int64x2_t __a, __Int32x2_t __b, int __c);
__Int32x4_t __builtin_aarch64_sqdmlslv4hi (__Int32x4_t __a, __Int16x4_t __b, __Int16x4_t __c);
__Int32x4_t __builtin_aarch64_sqdmlsl2v8hi (__Int32x4_t __a, __Int16x8_t __b, __Int16x8_t __c);
__Int32x4_t __builtin_aarch64_sqdmlsl2_lanev8hi (__Int32x4_t __a, __Int16x8_t __b, __Int16x4_t __c, int const __d);
__Int32x4_t __builtin_aarch64_sqdmlsl2_laneqv8hi (__Int32x4_t __a, __Int16x8_t __b, __Int16x8_t __c, int const __d);
__Int32x4_t __builtin_aarch64_sqdmlsl2_nv8hi (__Int32x4_t __a, __Int16x8_t __b, short int __c);
__Int32x4_t __builtin_aarch64_sqdmlsl_lanev4hi (__Int32x4_t __a, __Int16x4_t __b, __Int16x4_t __c, int const __d);
__Int32x4_t __builtin_aarch64_sqdmlsl_laneqv4hi (__Int32x4_t __a, __Int16x4_t __b, __Int16x8_t __c, int const __d);
__Int32x4_t __builtin_aarch64_sqdmlsl_nv4hi (__Int32x4_t __a, __Int16x4_t __b, short int __c);
__Int64x2_t __builtin_aarch64_sqdmlslv2si (__Int64x2_t __a, __Int32x2_t __b, __Int32x2_t __c);
__Int64x2_t __builtin_aarch64_sqdmlsl2v4si (__Int64x2_t __a, __Int32x4_t __b, __Int32x4_t __c);
__Int64x2_t __builtin_aarch64_sqdmlsl2_lanev4si (__Int64x2_t __a, __Int32x4_t __b, __Int32x2_t __c, int const __d);
__Int64x2_t __builtin_aarch64_sqdmlsl2_laneqv4si (__Int64x2_t __a, __Int32x4_t __b, __Int32x4_t __c, int const __d);
__Int64x2_t __builtin_aarch64_sqdmlsl2_nv4si (__Int64x2_t __a, __Int32x4_t __b, int __c);
__Int64x2_t __builtin_aarch64_sqdmlsl_lanev2si (__Int64x2_t __a, __Int32x2_t __b, __Int32x2_t __c, int const __d);
__Int64x2_t __builtin_aarch64_sqdmlsl_laneqv2si (__Int64x2_t __a, __Int32x2_t __b, __Int32x4_t __c, int const __d);
__Int64x2_t __builtin_aarch64_sqdmlsl_nv2si (__Int64x2_t __a, __Int32x2_t __b, int __c);
__Int16x4_t __builtin_aarch64_sqdmulh_lanev4hi (__Int16x4_t __a, __Int16x4_t __b, const int __c);
__Int32x2_t __builtin_aarch64_sqdmulh_lanev2si (__Int32x2_t __a, __Int32x2_t __b, const int __c);
__Int16x8_t __builtin_aarch64_sqdmulh_lanev8hi (__Int16x8_t __a, __Int16x4_t __b, const int __c);
__Int32x4_t __builtin_aarch64_sqdmulh_lanev4si (__Int32x4_t __a, __Int32x2_t __b, const int __c);
__Int32x4_t __builtin_aarch64_sqdmullv4hi (__Int16x4_t __a, __Int16x4_t __b);
__Int32x4_t __builtin_aarch64_sqdmull2v8hi (__Int16x8_t __a, __Int16x8_t __b);
__Int32x4_t __builtin_aarch64_sqdmull2_lanev8hi (__Int16x8_t __a, __Int16x4_t __b, int const __c);
__Int32x4_t __builtin_aarch64_sqdmull2_laneqv8hi (__Int16x8_t __a, __Int16x8_t __b, int const __c);
__Int32x4_t __builtin_aarch64_sqdmull2_nv8hi (__Int16x8_t __a, short int __b);
__Int32x4_t __builtin_aarch64_sqdmull_lanev4hi (__Int16x4_t __a, __Int16x4_t __b, int const __c);
__Int32x4_t __builtin_aarch64_sqdmull_laneqv4hi (__Int16x4_t __a, __Int16x8_t __b, int const __c);
__Int32x4_t __builtin_aarch64_sqdmull_nv4hi (__Int16x4_t __a, short int __b);
__Int64x2_t __builtin_aarch64_sqdmullv2si (__Int32x2_t __a, __Int32x2_t __b);
__Int64x2_t __builtin_aarch64_sqdmull2v4si (__Int32x4_t __a, __Int32x4_t __b);
__Int64x2_t __builtin_aarch64_sqdmull2_lanev4si (__Int32x4_t __a, __Int32x2_t __b, int const __c);
__Int64x2_t __builtin_aarch64_sqdmull2_laneqv4si (__Int32x4_t __a, __Int32x4_t __b, int const __c);
__Int64x2_t __builtin_aarch64_sqdmull2_nv4si (__Int32x4_t __a, int __b);
__Int64x2_t __builtin_aarch64_sqdmull_lanev2si (__Int32x2_t __a, __Int32x2_t __b, int const __c);
__Int64x2_t __builtin_aarch64_sqdmull_laneqv2si (__Int32x2_t __a, __Int32x4_t __b, int const __c);
__Int64x2_t __builtin_aarch64_sqdmull_nv2si (__Int32x2_t __a, int __b);
__Int16x4_t __builtin_aarch64_sqrdmulh_lanev4hi (__Int16x4_t __a, __Int16x4_t __b, const int __c);
__Int32x2_t __builtin_aarch64_sqrdmulh_lanev2si (__Int32x2_t __a, __Int32x2_t __b, const int __c);
__Int16x8_t __builtin_aarch64_sqrdmulh_lanev8hi (__Int16x8_t __a, __Int16x4_t __b, const int __c);
__Int32x4_t __builtin_aarch64_sqrdmulh_lanev4si (__Int32x4_t __a, __Int32x2_t __b, const int __c);
__Int8x8_t __builtin_aarch64_sqrshlv8qi (__Int8x8_t __a, __Int8x8_t __b);
__Int16x4_t __builtin_aarch64_sqrshlv4hi (__Int16x4_t __a, __Int16x4_t __b);
__Int32x2_t __builtin_aarch64_sqrshlv2si (__Int32x2_t __a, __Int32x2_t __b);
__Uint8x8_t __builtin_aarch64_uqrshlv8qi_uus (__Uint8x8_t __a, __Int8x8_t __b);
__Uint16x4_t __builtin_aarch64_uqrshlv4hi_uus (__Uint16x4_t __a, __Int16x4_t __b);
__Uint32x2_t __builtin_aarch64_uqrshlv2si_uus (__Uint32x2_t __a, __Int32x2_t __b);
__Int8x16_t __builtin_aarch64_sqrshlv16qi (__Int8x16_t __a, __Int8x16_t __b);
__Int16x8_t __builtin_aarch64_sqrshlv8hi (__Int16x8_t __a, __Int16x8_t __b);
__Int32x4_t __builtin_aarch64_sqrshlv4si (__Int32x4_t __a, __Int32x4_t __b);
__Int64x2_t __builtin_aarch64_sqrshlv2di (__Int64x2_t __a, __Int64x2_t __b);
__Uint8x16_t __builtin_aarch64_uqrshlv16qi_uus (__Uint8x16_t __a, __Int8x16_t __b);
__Uint16x8_t __builtin_aarch64_uqrshlv8hi_uus (__Uint16x8_t __a, __Int16x8_t __b);
__Uint32x4_t __builtin_aarch64_uqrshlv4si_uus (__Uint32x4_t __a, __Int32x4_t __b);
__Uint64x2_t __builtin_aarch64_uqrshlv2di_uus (__Uint64x2_t __a, __Int64x2_t __b);
__Uint8x8_t __builtin_aarch64_uqrshrn_nv8hi_uus (__Uint16x8_t __a, const int __b);
__Uint16x4_t __builtin_aarch64_uqrshrn_nv4si_uus (__Uint32x4_t __a, const int __b);
__Uint32x2_t __builtin_aarch64_uqrshrn_nv2di_uus (__Uint64x2_t __a, const int __b);
__Int8x8_t __builtin_aarch64_sqshlv8qi (__Int8x8_t __a, __Int8x8_t __b);
__Int16x4_t __builtin_aarch64_sqshlv4hi (__Int16x4_t __a, __Int16x4_t __b);
__Int32x2_t __builtin_aarch64_sqshlv2si (__Int32x2_t __a, __Int32x2_t __b);
__Uint8x8_t __builtin_aarch64_uqshlv8qi_uus (__Uint8x8_t __a, __Int8x8_t __b);
__Uint16x4_t __builtin_aarch64_uqshlv4hi_uus (__Uint16x4_t __a, __Int16x4_t __b);
__Uint32x2_t __builtin_aarch64_uqshlv2si_uus (__Uint32x2_t __a, __Int32x2_t __b);
__Int8x16_t __builtin_aarch64_sqshlv16qi (__Int8x16_t __a, __Int8x16_t __b);
__Int16x8_t __builtin_aarch64_sqshlv8hi (__Int16x8_t __a, __Int16x8_t __b);
__Int32x4_t __builtin_aarch64_sqshlv4si (__Int32x4_t __a, __Int32x4_t __b);
__Int64x2_t __builtin_aarch64_sqshlv2di (__Int64x2_t __a, __Int64x2_t __b);
__Uint8x16_t __builtin_aarch64_uqshlv16qi_uus (__Uint8x16_t __a, __Int8x16_t __b);
__Uint16x8_t __builtin_aarch64_uqshlv8hi_uus (__Uint16x8_t __a, __Int16x8_t __b);
__Uint32x4_t __builtin_aarch64_uqshlv4si_uus (__Uint32x4_t __a, __Int32x4_t __b);
__Uint64x2_t __builtin_aarch64_uqshlv2di_uus (__Uint64x2_t __a, __Int64x2_t __b);
__Uint8x8_t __builtin_aarch64_uqshl_nv8qi_uus (__Uint8x8_t __a, const int __b);
__Uint16x4_t __builtin_aarch64_uqshl_nv4hi_uus (__Uint16x4_t __a, const int __b);
__Uint32x2_t __builtin_aarch64_uqshl_nv2si_uus (__Uint32x2_t __a, const int __b);
__Uint8x16_t __builtin_aarch64_uqshl_nv16qi_uus (__Uint8x16_t __a, const int __b);
__Uint16x8_t __builtin_aarch64_uqshl_nv8hi_uus (__Uint16x8_t __a, const int __b);
__Uint32x4_t __builtin_aarch64_uqshl_nv4si_uus (__Uint32x4_t __a, const int __b);
__Uint64x2_t __builtin_aarch64_uqshl_nv2di_uus (__Uint64x2_t __a, const int __b);
__Uint8x8_t __builtin_aarch64_sqshlu_nv8qi_uss (__Int8x8_t __a, const int __b);
__Uint16x4_t __builtin_aarch64_sqshlu_nv4hi_uss (__Int16x4_t __a, const int __b);
__Uint32x2_t __builtin_aarch64_sqshlu_nv2si_uss (__Int32x2_t __a, const int __b);
__Uint8x16_t __builtin_aarch64_sqshlu_nv16qi_uss (__Int8x16_t __a, const int __b);
__Uint16x8_t __builtin_aarch64_sqshlu_nv8hi_uss (__Int16x8_t __a, const int __b);
__Uint32x4_t __builtin_aarch64_sqshlu_nv4si_uss (__Int32x4_t __a, const int __b);
__Uint64x2_t __builtin_aarch64_sqshlu_nv2di_uss (__Int64x2_t __a, const int __b);
__Uint8x8_t __builtin_aarch64_uqshrn_nv8hi_uus (__Uint16x8_t __a, const int __b);
__Uint16x4_t __builtin_aarch64_uqshrn_nv4si_uus (__Uint32x4_t __a, const int __b);
__Uint32x2_t __builtin_aarch64_uqshrn_nv2di_uus (__Uint64x2_t __a, const int __b);
__Int8x8_t __builtin_aarch64_rbitv8qi (__Int8x8_t __a);
__Int8x16_t __builtin_aarch64_rbitv16qi (__Int8x16_t __a);
__Float32x2_t __builtin_aarch64_frecpev2sf (__Float32x2_t __a);
__Float32x4_t __builtin_aarch64_frecpev4sf (__Float32x4_t __a);
__Float64x2_t __builtin_aarch64_frecpev2df (__Float64x2_t __a);
__Float32x2_t __builtin_aarch64_frecpsv2sf (__Float32x2_t __a, __Float32x2_t __b);
__Float32x4_t __builtin_aarch64_frecpsv4sf (__Float32x4_t __a, __Float32x4_t __b);
__Float64x2_t __builtin_aarch64_frecpsv2df (__Float64x2_t __a, __Float64x2_t __b);
__Float32x2_t __builtin_aarch64_btruncv2sf (__Float32x2_t __a);
__Float32x4_t __builtin_aarch64_btruncv4sf (__Float32x4_t __a);
__Float64x2_t __builtin_aarch64_btruncv2df (__Float64x2_t __a);
__Float32x2_t __builtin_aarch64_roundv2sf (__Float32x2_t __a);
__Float32x4_t __builtin_aarch64_roundv4sf (__Float32x4_t __a);
__Float64x2_t __builtin_aarch64_roundv2df (__Float64x2_t __a);
__Float32x2_t __builtin_aarch64_nearbyintv2sf (__Float32x2_t __a);
__Float32x4_t __builtin_aarch64_nearbyintv4sf (__Float32x4_t __a);
__Float64x2_t __builtin_aarch64_nearbyintv2df (__Float64x2_t __a);
__Float32x2_t __builtin_aarch64_floorv2sf (__Float32x2_t __a);
__Float32x4_t __builtin_aarch64_floorv4sf (__Float32x4_t __a);
__Float64x2_t __builtin_aarch64_floorv2df (__Float64x2_t __a);
__Float32x2_t __builtin_aarch64_frintnv2sf (__Float32x2_t __a);
__Float32x4_t __builtin_aarch64_frintnv4sf (__Float32x4_t __a);
__Float64x2_t __builtin_aarch64_frintnv2df (__Float64x2_t __a);
__Float32x2_t __builtin_aarch64_ceilv2sf (__Float32x2_t __a);
__Float32x4_t __builtin_aarch64_ceilv4sf (__Float32x4_t __a);
__Float64x2_t __builtin_aarch64_ceilv2df (__Float64x2_t __a);
__Float32x2_t __builtin_aarch64_rintv2sf (__Float32x2_t __a);
__Float32x4_t __builtin_aarch64_rintv4sf (__Float32x4_t __a);
__Float64x2_t __builtin_aarch64_rintv2df (__Float64x2_t __a);
__Uint8x8_t __builtin_aarch64_urshlv8qi_uus (__Uint8x8_t __a, __Int8x8_t __b);
__Uint16x4_t __builtin_aarch64_urshlv4hi_uus (__Uint16x4_t __a, __Int16x4_t __b);
__Uint32x2_t __builtin_aarch64_urshlv2si_uus (__Uint32x2_t __a, __Int32x2_t __b);
__Uint8x16_t __builtin_aarch64_urshlv16qi_uus (__Uint8x16_t __a, __Int8x16_t __b);
__Uint16x8_t __builtin_aarch64_urshlv8hi_uus (__Uint16x8_t __a, __Int16x8_t __b);
__Uint32x4_t __builtin_aarch64_urshlv4si_uus (__Uint32x4_t __a, __Int32x4_t __b);
__Uint64x2_t __builtin_aarch64_urshlv2di_uus (__Uint64x2_t __a, __Int64x2_t __b);
__Uint8x8_t __builtin_aarch64_urshr_nv8qi_uus (__Uint8x8_t __a, const int __b);
__Uint16x4_t __builtin_aarch64_urshr_nv4hi_uus (__Uint16x4_t __a, const int __b);
__Uint32x2_t __builtin_aarch64_urshr_nv2si_uus (__Uint32x2_t __a, const int __b);
__Uint8x16_t __builtin_aarch64_urshr_nv16qi_uus (__Uint8x16_t __a, const int __b);
__Uint16x8_t __builtin_aarch64_urshr_nv8hi_uus (__Uint16x8_t __a, const int __b);
__Uint32x4_t __builtin_aarch64_urshr_nv4si_uus (__Uint32x4_t __a, const int __b);
__Uint64x2_t __builtin_aarch64_urshr_nv2di_uus (__Uint64x2_t __a, const int __b);
__Uint8x8_t __builtin_aarch64_ursra_nv8qi_uuus (__Uint8x8_t __a, __Uint8x8_t __b, const int __c);
__Uint16x4_t __builtin_aarch64_ursra_nv4hi_uuus (__Uint16x4_t __a, __Uint16x4_t __b, const int __c);
__Uint32x2_t __builtin_aarch64_ursra_nv2si_uuus (__Uint32x2_t __a, __Uint32x2_t __b, const int __c);
__Uint8x16_t __builtin_aarch64_ursra_nv16qi_uuus (__Uint8x16_t __a, __Uint8x16_t __b, const int __c);
__Uint16x8_t __builtin_aarch64_ursra_nv8hi_uuus (__Uint16x8_t __a, __Uint16x8_t __b, const int __c);
__Uint32x4_t __builtin_aarch64_ursra_nv4si_uuus (__Uint32x4_t __a, __Uint32x4_t __b, const int __c);
__Uint64x2_t __builtin_aarch64_ursra_nv2di_uuus (__Uint64x2_t __a, __Uint64x2_t __b, const int __c);
__Uint32x4_t __builtin_aarch64_crypto_sha1cv4si_uuuu (__Uint32x4_t hash_abcd, unsigned int hash_e, __Uint32x4_t wk);
__Uint32x4_t __builtin_aarch64_crypto_sha1mv4si_uuuu (__Uint32x4_t hash_abcd, unsigned int hash_e, __Uint32x4_t wk);
__Uint32x4_t __builtin_aarch64_crypto_sha1pv4si_uuuu (__Uint32x4_t hash_abcd, unsigned int hash_e, __Uint32x4_t wk);
__Uint32x4_t __builtin_aarch64_crypto_sha1su0v4si_uuuu (__Uint32x4_t w0_3, __Uint32x4_t w4_7, __Uint32x4_t w8_11);
__Uint32x4_t __builtin_aarch64_crypto_sha1su1v4si_uuu (__Uint32x4_t tw0_3, __Uint32x4_t w12_15);
__Uint32x4_t __builtin_aarch64_crypto_sha256hv4si_uuuu (__Uint32x4_t hash_abcd, __Uint32x4_t hash_efgh, __Uint32x4_t wk);
__Uint32x4_t __builtin_aarch64_crypto_sha256h2v4si_uuuu (__Uint32x4_t hash_efgh, __Uint32x4_t hash_abcd, __Uint32x4_t wk);
__Uint32x4_t __builtin_aarch64_crypto_sha256su0v4si_uuu (__Uint32x4_t w0_3, __Uint32x4_t w4_7);
__Uint32x4_t __builtin_aarch64_crypto_sha256su1v4si_uuuu (__Uint32x4_t tw0_3, __Uint32x4_t w8_11, __Uint32x4_t w12_15);
__Poly128_t __builtin_aarch64_crypto_pmullv2di_ppp (__Poly64x2_t a, __Poly64x2_t b);
__Int8x8_t __builtin_aarch64_sshlv8qi (__Int8x8_t __a, __Int8x8_t __b);
__Int16x4_t __builtin_aarch64_sshlv4hi (__Int16x4_t __a, __Int16x4_t __b);
__Int32x2_t __builtin_aarch64_sshlv2si (__Int32x2_t __a, __Int32x2_t __b);
__Uint8x8_t __builtin_aarch64_ushlv8qi_uus (__Uint8x8_t __a, __Int8x8_t __b);
__Uint16x4_t __builtin_aarch64_ushlv4hi_uus (__Uint16x4_t __a, __Int16x4_t __b);
__Uint32x2_t __builtin_aarch64_ushlv2si_uus (__Uint32x2_t __a, __Int32x2_t __b);
__Int8x16_t __builtin_aarch64_sshlv16qi (__Int8x16_t __a, __Int8x16_t __b);
__Int16x8_t __builtin_aarch64_sshlv8hi (__Int16x8_t __a, __Int16x8_t __b);
__Int32x4_t __builtin_aarch64_sshlv4si (__Int32x4_t __a, __Int32x4_t __b);
__Int64x2_t __builtin_aarch64_sshlv2di (__Int64x2_t __a, __Int64x2_t __b);
__Uint8x16_t __builtin_aarch64_ushlv16qi_uus (__Uint8x16_t __a, __Int8x16_t __b);
__Uint16x8_t __builtin_aarch64_ushlv8hi_uus (__Uint16x8_t __a, __Int16x8_t __b);
__Uint32x4_t __builtin_aarch64_ushlv4si_uus (__Uint32x4_t __a, __Int32x4_t __b);
__Uint64x2_t __builtin_aarch64_ushlv2di_uus (__Uint64x2_t __a, __Int64x2_t __b);
__Int16x8_t __builtin_aarch64_sshll2_nv16qi (__Int8x16_t __a, const int __b);
__Int32x4_t __builtin_aarch64_sshll2_nv8hi (__Int16x8_t __a, const int __b);
__Int64x2_t __builtin_aarch64_sshll2_nv4si (__Int32x4_t __a, const int __b);
__Int16x8_t __builtin_aarch64_sshll_nv8qi (__Int8x8_t __a, const int __b);
__Int32x4_t __builtin_aarch64_sshll_nv4hi (__Int16x4_t __a, const int __b);
__Int64x2_t __builtin_aarch64_sshll_nv2si (__Int32x2_t __a, const int __b);
__Uint16x8_t __builtin_aarch64_ushll_nv8qi_uus (__Uint8x8_t __a, const int __b);
__Uint32x4_t __builtin_aarch64_ushll_nv4hi_uus (__Uint16x4_t __a, const int __b);
__Uint64x2_t __builtin_aarch64_ushll_nv2si_uus (__Uint32x2_t __a, const int __b);
__Int32x4_t __builtin_aarch64_lshrv4si (__Int32x4_t __a, const int __b);
__Int64x2_t __builtin_aarch64_lshrv2di (__Int64x2_t __a, const int __b);
__Uint8x8_t __builtin_aarch64_usli_nv8qi_uuus (__Uint8x8_t __a, __Uint8x8_t __b, const int __c);
__Uint16x4_t __builtin_aarch64_usli_nv4hi_uuus (__Uint16x4_t __a, __Uint16x4_t __b, const int __c);
__Uint32x2_t __builtin_aarch64_usli_nv2si_uuus (__Uint32x2_t __a, __Uint32x2_t __b, const int __c);
__Uint8x16_t __builtin_aarch64_usli_nv16qi_uuus (__Uint8x16_t __a, __Uint8x16_t __b, const int __c);
__Uint16x8_t __builtin_aarch64_usli_nv8hi_uuus (__Uint16x8_t __a, __Uint16x8_t __b, const int __c);
__Uint32x4_t __builtin_aarch64_usli_nv4si_uuus (__Uint32x4_t __a, __Uint32x4_t __b, const int __c);
__Uint64x2_t __builtin_aarch64_usli_nv2di_uuus (__Uint64x2_t __a, __Uint64x2_t __b, const int __c);
__Uint8x8_t __builtin_aarch64_usqaddv8qi_uus (__Uint8x8_t __a, __Int8x8_t __b);
__Uint16x4_t __builtin_aarch64_usqaddv4hi_uus (__Uint16x4_t __a, __Int16x4_t __b);
__Uint32x2_t __builtin_aarch64_usqaddv2si_uus (__Uint32x2_t __a, __Int32x2_t __b);
__Uint8x16_t __builtin_aarch64_usqaddv16qi_uus (__Uint8x16_t __a, __Int8x16_t __b);
__Uint16x8_t __builtin_aarch64_usqaddv8hi_uus (__Uint16x8_t __a, __Int16x8_t __b);
__Uint32x4_t __builtin_aarch64_usqaddv4si_uus (__Uint32x4_t __a, __Int32x4_t __b);
__Uint64x2_t __builtin_aarch64_usqaddv2di_uus (__Uint64x2_t __a, __Int64x2_t __b);
__Float32x2_t __builtin_aarch64_sqrtv2sf (__Float32x2_t a);
__Float32x4_t __builtin_aarch64_sqrtv4sf (__Float32x4_t a);
__Float64x2_t __builtin_aarch64_sqrtv2df (__Float64x2_t a);
__Uint8x8_t __builtin_aarch64_usra_nv8qi_uuus (__Uint8x8_t __a, __Uint8x8_t __b, const int __c);
__Uint16x4_t __builtin_aarch64_usra_nv4hi_uuus (__Uint16x4_t __a, __Uint16x4_t __b, const int __c);
__Uint32x2_t __builtin_aarch64_usra_nv2si_uuus (__Uint32x2_t __a, __Uint32x2_t __b, const int __c);
__Uint8x16_t __builtin_aarch64_usra_nv16qi_uuus (__Uint8x16_t __a, __Uint8x16_t __b, const int __c);
__Uint16x8_t __builtin_aarch64_usra_nv8hi_uuus (__Uint16x8_t __a, __Uint16x8_t __b, const int __c);
__Uint32x4_t __builtin_aarch64_usra_nv4si_uuus (__Uint32x4_t __a, __Uint32x4_t __b, const int __c);
__Uint64x2_t __builtin_aarch64_usra_nv2di_uuus (__Uint64x2_t __a, __Uint64x2_t __b, const int __c);
__Uint8x8_t __builtin_aarch64_usri_nv8qi_uuus (__Uint8x8_t __a, __Uint8x8_t __b, const int __c);
__Uint16x4_t __builtin_aarch64_usri_nv4hi_uuus (__Uint16x4_t __a, __Uint16x4_t __b, const int __c);
__Uint32x2_t __builtin_aarch64_usri_nv2si_uuus (__Uint32x2_t __a, __Uint32x2_t __b, const int __c);
__Uint8x16_t __builtin_aarch64_usri_nv16qi_uuus (__Uint8x16_t __a, __Uint8x16_t __b, const int __c);
__Uint16x8_t __builtin_aarch64_usri_nv8hi_uuus (__Uint16x8_t __a, __Uint16x8_t __b, const int __c);
__Uint32x4_t __builtin_aarch64_usri_nv4si_uuus (__Uint32x4_t __a, __Uint32x4_t __b, const int __c);
__Uint64x2_t __builtin_aarch64_usri_nv2di_uuus (__Uint64x2_t __a, __Uint64x2_t __b, const int __c);
__Int8x8_t __builtin_aarch64_suqaddv8qi_ssu (__Int8x8_t __a, __Uint8x8_t __b);
__Int16x4_t __builtin_aarch64_suqaddv4hi_ssu (__Int16x4_t __a, __Uint16x4_t __b);
__Int32x2_t __builtin_aarch64_suqaddv2si_ssu (__Int32x2_t __a, __Uint32x2_t __b);
__Int8x16_t __builtin_aarch64_suqaddv16qi_ssu (__Int8x16_t __a, __Uint8x16_t __b);
__Int16x8_t __builtin_aarch64_suqaddv8hi_ssu (__Int16x8_t __a, __Uint16x8_t __b);
__Int32x4_t __builtin_aarch64_suqaddv4si_ssu (__Int32x4_t __a, __Uint32x4_t __b);
__Int64x2_t __builtin_aarch64_suqaddv2di_ssu (__Int64x2_t __a, __Uint64x2_t __b);
__Poly128_t __builtin_aarch64_crypto_pmulldi_ppp (__Poly64_t a, __Poly64_t b);
__Int8x8_t __builtin_aarch64_tbx4v8qi (__Int8x8_t, __builtin_aarch64_simd_oi, __Int8x8_t);
//end CMPCPP-6477
void __builtin_aarch64_st2_lanev4hf(__fp16*, __builtin_aarch64_simd_oi, int);
void __builtin_aarch64_st2_lanev2sf(float*, __builtin_aarch64_simd_oi, int);
void __builtin_aarch64_st2_lanedf(double*, __builtin_aarch64_simd_oi, int);
void __builtin_aarch64_st2_lanev8qi(signed char*, __builtin_aarch64_simd_oi, int);
void __builtin_aarch64_st2_lanev4hi(short int*, __builtin_aarch64_simd_oi, int);
void __builtin_aarch64_st2_lanedi(long int*, __builtin_aarch64_simd_oi, int);
void __builtin_aarch64_st2_lanev2si(int*, __builtin_aarch64_simd_oi, int);
void __builtin_aarch64_st3_lanev4hf(__fp16*, __builtin_aarch64_simd_ci, int);
void __builtin_aarch64_st3_lanev2sf(float*, __builtin_aarch64_simd_ci, int);
void __builtin_aarch64_st3_lanedf(double*, __builtin_aarch64_simd_ci, int);
void __builtin_aarch64_st3_lanev8qi(signed char*, __builtin_aarch64_simd_ci, int);
void __builtin_aarch64_st3_lanev4hi(short int*, __builtin_aarch64_simd_ci, int);
void __builtin_aarch64_st3_lanedi(long int*, __builtin_aarch64_simd_ci, int);
void __builtin_aarch64_st3_lanev2si(int*, __builtin_aarch64_simd_ci, int);
void __builtin_aarch64_st4_lanev4hf(__fp16*, __builtin_aarch64_simd_xi, int);
void __builtin_aarch64_st4_lanev2sf(float*, __builtin_aarch64_simd_xi, int);
void __builtin_aarch64_st4_lanedf(double*, __builtin_aarch64_simd_xi, int);
void __builtin_aarch64_st4_lanev8qi(signed char*, __builtin_aarch64_simd_xi, int);
void __builtin_aarch64_st4_lanev4hi(short int*, __builtin_aarch64_simd_xi, int);
void __builtin_aarch64_st4_lanedi(long int*, __builtin_aarch64_simd_xi, int);
void __builtin_aarch64_st4_lanev2si(int*, __builtin_aarch64_simd_xi, int);
__builtin_aarch64_simd_oi __builtin_aarch64_ld2_lanev4hf(const __fp16*, __builtin_aarch64_simd_oi, int);
__builtin_aarch64_simd_oi __builtin_aarch64_ld2_lanev2sf(const float*, __builtin_aarch64_simd_oi, int);
__builtin_aarch64_simd_oi __builtin_aarch64_ld2_lanedf(const double*, __builtin_aarch64_simd_oi, int);
__builtin_aarch64_simd_oi __builtin_aarch64_ld2_lanev8qi(const signed char*, __builtin_aarch64_simd_oi, int);
__builtin_aarch64_simd_oi __builtin_aarch64_ld2_lanev4hi(const short int*, __builtin_aarch64_simd_oi, int);
__builtin_aarch64_simd_oi __builtin_aarch64_ld2_lanedi(const long int*, __builtin_aarch64_simd_oi, int);
__builtin_aarch64_simd_oi __builtin_aarch64_ld2_lanev2si(const int*, __builtin_aarch64_simd_oi, int);
__builtin_aarch64_simd_ci __builtin_aarch64_ld3_lanev4hf(const __fp16*, __builtin_aarch64_simd_ci, int);
__builtin_aarch64_simd_ci __builtin_aarch64_ld3_lanev2sf(const float*, __builtin_aarch64_simd_ci, int);
__builtin_aarch64_simd_ci __builtin_aarch64_ld3_lanedf(const double*, __builtin_aarch64_simd_ci, int);
__builtin_aarch64_simd_ci __builtin_aarch64_ld3_lanev8qi(const signed char*, __builtin_aarch64_simd_ci, int);
__builtin_aarch64_simd_ci __builtin_aarch64_ld3_lanev4hi(const short int*, __builtin_aarch64_simd_ci, int);
__builtin_aarch64_simd_ci __builtin_aarch64_ld3_lanedi(const long int*, __builtin_aarch64_simd_ci, int);
__builtin_aarch64_simd_ci __builtin_aarch64_ld3_lanev2si(const int*, __builtin_aarch64_simd_ci, int);
__builtin_aarch64_simd_xi __builtin_aarch64_ld4_lanev4hf(const __fp16*, __builtin_aarch64_simd_xi, int);
__builtin_aarch64_simd_xi __builtin_aarch64_ld4_lanev2sf(const float*, __builtin_aarch64_simd_xi, int);
__builtin_aarch64_simd_xi __builtin_aarch64_ld4_lanedf(const double*, __builtin_aarch64_simd_xi, int);
__builtin_aarch64_simd_xi __builtin_aarch64_ld4_lanev8qi(const signed char*, __builtin_aarch64_simd_xi, int);
__builtin_aarch64_simd_xi __builtin_aarch64_ld4_lanev4hi(const short int*, __builtin_aarch64_simd_xi, int);
__builtin_aarch64_simd_xi __builtin_aarch64_ld4_lanedi(const long int*, __builtin_aarch64_simd_xi, int);
__builtin_aarch64_simd_xi __builtin_aarch64_ld4_lanev2si(const int*, __builtin_aarch64_simd_xi, int);
__builtin_aarch64_simd_oi __builtin_aarch64_ld2_lanev16qi(const signed char*, __builtin_aarch64_simd_oi, int);
__builtin_aarch64_simd_oi __builtin_aarch64_ld2_lanev2df(const double*, __builtin_aarch64_simd_oi, int);
__builtin_aarch64_simd_oi __builtin_aarch64_ld2_lanev2di(const long int*, __builtin_aarch64_simd_oi, int);
__builtin_aarch64_simd_oi __builtin_aarch64_ld2_lanev4sf(const float*, __builtin_aarch64_simd_oi, int);
__builtin_aarch64_simd_oi __builtin_aarch64_ld2_lanev4si(const int*, __builtin_aarch64_simd_oi, int);
__builtin_aarch64_simd_oi __builtin_aarch64_ld2_lanev8hf(const __fp16*, __builtin_aarch64_simd_oi, int);
__builtin_aarch64_simd_oi __builtin_aarch64_ld2_lanev8hi(const short int*, __builtin_aarch64_simd_oi, int);
__builtin_aarch64_simd_ci __builtin_aarch64_ld3_lanev16qi(const signed char*, __builtin_aarch64_simd_ci, int);
__builtin_aarch64_simd_ci __builtin_aarch64_ld3_lanev2df(const double*, __builtin_aarch64_simd_ci, int);
__builtin_aarch64_simd_ci __builtin_aarch64_ld3_lanev2di(const long int*, __builtin_aarch64_simd_ci, int);
__builtin_aarch64_simd_ci __builtin_aarch64_ld3_lanev4sf(const float*, __builtin_aarch64_simd_ci, int);
__builtin_aarch64_simd_ci __builtin_aarch64_ld3_lanev4si(const int*, __builtin_aarch64_simd_ci, int);
__builtin_aarch64_simd_ci __builtin_aarch64_ld3_lanev8hf(const __fp16*, __builtin_aarch64_simd_ci, int);
__builtin_aarch64_simd_ci __builtin_aarch64_ld3_lanev8hi(const short int*, __builtin_aarch64_simd_ci, int);
__builtin_aarch64_simd_xi __builtin_aarch64_ld4_lanev16qi(const signed char*, __builtin_aarch64_simd_xi, int);
__builtin_aarch64_simd_xi __builtin_aarch64_ld4_lanev2df(const double*, __builtin_aarch64_simd_xi, int);
__builtin_aarch64_simd_xi __builtin_aarch64_ld4_lanev2di(const long int*, __builtin_aarch64_simd_xi, int);
__builtin_aarch64_simd_xi __builtin_aarch64_ld4_lanev4sf(const float*, __builtin_aarch64_simd_xi, int);
__builtin_aarch64_simd_xi __builtin_aarch64_ld4_lanev4si(const int*, __builtin_aarch64_simd_xi, int);
__builtin_aarch64_simd_xi __builtin_aarch64_ld4_lanev8hf(const __fp16*, __builtin_aarch64_simd_xi, int);
__builtin_aarch64_simd_xi __builtin_aarch64_ld4_lanev8hi(const short int*, __builtin_aarch64_simd_xi, int);
void __builtin_aarch64_st2_lanev16qi(signed char*, __builtin_aarch64_simd_oi, int);
void __builtin_aarch64_st2_lanev2df(double*, __builtin_aarch64_simd_oi, int);
void __builtin_aarch64_st2_lanev2di(long int*, __builtin_aarch64_simd_oi, int);
void __builtin_aarch64_st2_lanev4sf(float*, __builtin_aarch64_simd_oi, int);
void __builtin_aarch64_st2_lanev4si(int*, __builtin_aarch64_simd_oi, int);
void __builtin_aarch64_st2_lanev8hf(__fp16*, __builtin_aarch64_simd_oi, int);
void __builtin_aarch64_st2_lanev8hi(short int*, __builtin_aarch64_simd_oi, int);
void __builtin_aarch64_st3_lanev16qi(signed char*, __builtin_aarch64_simd_ci, int);
void __builtin_aarch64_st3_lanev2df(double*, __builtin_aarch64_simd_ci, int);
void __builtin_aarch64_st3_lanev2di(long int*, __builtin_aarch64_simd_ci, int);
void __builtin_aarch64_st3_lanev4sf(float*, __builtin_aarch64_simd_ci, int);
void __builtin_aarch64_st3_lanev4si(int*, __builtin_aarch64_simd_ci, int);
void __builtin_aarch64_st3_lanev8hf(__fp16*, __builtin_aarch64_simd_ci, int);
void __builtin_aarch64_st3_lanev8hi(short int*, __builtin_aarch64_simd_ci, int);
void __builtin_aarch64_st4_lanev16qi(signed char*, __builtin_aarch64_simd_xi, int);
void __builtin_aarch64_st4_lanev2df(double*, __builtin_aarch64_simd_xi, int);
void __builtin_aarch64_st4_lanev2di(long int*, __builtin_aarch64_simd_xi, int);
void __builtin_aarch64_st4_lanev4sf(float*, __builtin_aarch64_simd_xi, int);
void __builtin_aarch64_st4_lanev4si(int*, __builtin_aarch64_simd_xi, int);
void __builtin_aarch64_st4_lanev8hf(__fp16*, __builtin_aarch64_simd_xi, int);
void __builtin_aarch64_st4_lanev8hi(short int*, __builtin_aarch64_simd_xi, int);
__fp16 __builtin_aarch64_abshf(__fp16);
__Float16x4_t __builtin_aarch64_absv4hf(__Float16x4_t);
__Float16x8_t __builtin_aarch64_absv8hf(__Float16x8_t);
long int __builtin_aarch64_addpdi(__Int64x2_t);
long int __builtin_aarch64_ashldi(long int, signed char);
long int __builtin_aarch64_ashr_simddi(long int, int);
__Int8x16_t __builtin_aarch64_bcaxqv16qi(__Int8x16_t, __Int8x16_t, __Int8x16_t);
__Uint8x16_t __builtin_aarch64_bcaxqv16qi_uuuu(__Uint8x16_t, __Uint8x16_t, __Uint8x16_t);
__Int64x2_t __builtin_aarch64_bcaxqv2di(__Int64x2_t, __Int64x2_t, __Int64x2_t);
__Uint64x2_t __builtin_aarch64_bcaxqv2di_uuuu(__Uint64x2_t, __Uint64x2_t, __Uint64x2_t);
__Int32x4_t __builtin_aarch64_bcaxqv4si(__Int32x4_t, __Int32x4_t, __Int32x4_t);
__Uint32x4_t __builtin_aarch64_bcaxqv4si_uuuu(__Uint32x4_t, __Uint32x4_t, __Uint32x4_t);
__Int16x8_t __builtin_aarch64_bcaxqv8hi(__Int16x8_t, __Int16x8_t, __Int16x8_t);
__Uint16x8_t __builtin_aarch64_bcaxqv8hi_uuuu(__Uint16x8_t, __Uint16x8_t, __Uint16x8_t);
__fp16 __builtin_aarch64_btrunchf(__fp16);
__Float16x4_t __builtin_aarch64_btruncv4hf(__Float16x4_t);
__Float16x8_t __builtin_aarch64_btruncv8hf(__Float16x8_t);
__fp16 __builtin_aarch64_ceilhf(__fp16);
__Float16x4_t __builtin_aarch64_ceilv4hf(__Float16x4_t);
__Float16x8_t __builtin_aarch64_ceilv8hf(__Float16x8_t);
short unsigned int __builtin_aarch64_cmeqhf_uss(__fp16, __fp16);
__Uint16x4_t __builtin_aarch64_cmeqv4hf_uss(__Float16x4_t, __Float16x4_t);
__Uint16x8_t __builtin_aarch64_cmeqv8hf_uss(__Float16x8_t, __Float16x8_t);
short unsigned int __builtin_aarch64_cmgehf_uss(__fp16, __fp16);
__Uint16x4_t __builtin_aarch64_cmgev4hf_uss(__Float16x4_t, __Float16x4_t);
__Uint16x8_t __builtin_aarch64_cmgev8hf_uss(__Float16x8_t, __Float16x8_t);
short unsigned int __builtin_aarch64_cmgthf_uss(__fp16, __fp16);
__Uint16x4_t __builtin_aarch64_cmgtv4hf_uss(__Float16x4_t, __Float16x4_t);
__Uint16x8_t __builtin_aarch64_cmgtv8hf_uss(__Float16x8_t, __Float16x8_t);
short unsigned int __builtin_aarch64_cmlehf_uss(__fp16, __fp16);
__Uint16x4_t __builtin_aarch64_cmlev4hf_uss(__Float16x4_t, __Float16x4_t);
__Uint16x8_t __builtin_aarch64_cmlev8hf_uss(__Float16x8_t, __Float16x8_t);
short unsigned int __builtin_aarch64_cmlthf_uss(__fp16, __fp16);
__Uint16x4_t __builtin_aarch64_cmltv4hf_uss(__Float16x4_t, __Float16x4_t);
__Uint16x8_t __builtin_aarch64_cmltv8hf_uss(__Float16x8_t, __Float16x8_t);
__Poly64x2_t __builtin_aarch64_combinedi_ppp(__Poly64_t, __Poly64_t);
unsigned int __builtin_aarch64_crc32b(unsigned int, unsigned char);
unsigned int __builtin_aarch64_crc32cb(unsigned int, unsigned char);
unsigned int __builtin_aarch64_crc32ch(unsigned int, short unsigned int);
unsigned int __builtin_aarch64_crc32cw(unsigned int, unsigned int);
unsigned int __builtin_aarch64_crc32cx(unsigned int, long unsigned int);
unsigned int __builtin_aarch64_crc32h(unsigned int, short unsigned int);
unsigned int __builtin_aarch64_crc32w(unsigned int, unsigned int);
unsigned int __builtin_aarch64_crc32x(unsigned int, long unsigned int);
unsigned int __builtin_aarch64_crypto_sha1hsi_uu(unsigned int);
__Uint64x2_t __builtin_aarch64_crypto_sha512h2qv2di_uuuu(__Uint64x2_t, __Uint64x2_t, __Uint64x2_t);
__Uint64x2_t __builtin_aarch64_crypto_sha512hqv2di_uuuu(__Uint64x2_t, __Uint64x2_t, __Uint64x2_t);
__Uint64x2_t __builtin_aarch64_crypto_sha512su0qv2di_uuu(__Uint64x2_t, __Uint64x2_t);
__Uint64x2_t __builtin_aarch64_crypto_sha512su1qv2di_uuuu(__Uint64x2_t, __Uint64x2_t, __Uint64x2_t);
__Int8x16_t __builtin_aarch64_eor3qv16qi(__Int8x16_t, __Int8x16_t, __Int8x16_t);
__Uint8x16_t __builtin_aarch64_eor3qv16qi_uuuu(__Uint8x16_t, __Uint8x16_t, __Uint8x16_t);
__Int64x2_t __builtin_aarch64_eor3qv2di(__Int64x2_t, __Int64x2_t, __Int64x2_t);
__Uint64x2_t __builtin_aarch64_eor3qv2di_uuuu(__Uint64x2_t, __Uint64x2_t, __Uint64x2_t);
__Int32x4_t __builtin_aarch64_eor3qv4si(__Int32x4_t, __Int32x4_t, __Int32x4_t);
__Uint32x4_t __builtin_aarch64_eor3qv4si_uuuu(__Uint32x4_t, __Uint32x4_t, __Uint32x4_t);
__Int16x8_t __builtin_aarch64_eor3qv8hi(__Int16x8_t, __Int16x8_t, __Int16x8_t);
__Uint16x8_t __builtin_aarch64_eor3qv8hi_uuuu(__Uint16x8_t, __Uint16x8_t, __Uint16x8_t);
double __builtin_aarch64_fabddf(double, double);
__fp16 __builtin_aarch64_fabdhf(__fp16, __fp16);
float __builtin_aarch64_fabdsf(float, float);
__Float64x2_t __builtin_aarch64_fabdv2df(__Float64x2_t, __Float64x2_t);
__Float32x2_t __builtin_aarch64_fabdv2sf(__Float32x2_t, __Float32x2_t);
__Float16x4_t __builtin_aarch64_fabdv4hf(__Float16x4_t, __Float16x4_t);
__Float32x4_t __builtin_aarch64_fabdv4sf(__Float32x4_t, __Float32x4_t);
__Float16x8_t __builtin_aarch64_fabdv8hf(__Float16x8_t, __Float16x8_t);
short unsigned int __builtin_aarch64_facgehf_uss(__fp16, __fp16);
__Uint16x4_t __builtin_aarch64_facgev4hf_uss(__Float16x4_t, __Float16x4_t);
__Uint16x8_t __builtin_aarch64_facgev8hf_uss(__Float16x8_t, __Float16x8_t);
short unsigned int __builtin_aarch64_facgthf_uss(__fp16, __fp16);
__Uint16x4_t __builtin_aarch64_facgtv4hf_uss(__Float16x4_t, __Float16x4_t);
__Uint16x8_t __builtin_aarch64_facgtv8hf_uss(__Float16x8_t, __Float16x8_t);
short unsigned int __builtin_aarch64_faclehf_uss(__fp16, __fp16);
__Uint16x4_t __builtin_aarch64_faclev4hf_uss(__Float16x4_t, __Float16x4_t);
__Uint16x8_t __builtin_aarch64_faclev8hf_uss(__Float16x8_t, __Float16x8_t);
short unsigned int __builtin_aarch64_faclthf_uss(__fp16, __fp16);
__Uint16x4_t __builtin_aarch64_facltv4hf_uss(__Float16x4_t, __Float16x4_t);
__Uint16x8_t __builtin_aarch64_facltv8hf_uss(__Float16x8_t, __Float16x8_t);
__Float64x2_t __builtin_aarch64_faddpv2df(__Float64x2_t, __Float64x2_t);
__Float32x2_t __builtin_aarch64_faddpv2sf(__Float32x2_t, __Float32x2_t);
__Float16x4_t __builtin_aarch64_faddpv4hf(__Float16x4_t, __Float16x4_t);
__Float32x4_t __builtin_aarch64_faddpv4sf(__Float32x4_t, __Float32x4_t);
__Float16x8_t __builtin_aarch64_faddpv8hf(__Float16x8_t, __Float16x8_t);
__Float64x2_t __builtin_aarch64_fcadd270v2df(__Float64x2_t, __Float64x2_t);
__Float32x2_t __builtin_aarch64_fcadd270v2sf(__Float32x2_t, __Float32x2_t);
__Float16x4_t __builtin_aarch64_fcadd270v4hf(__Float16x4_t, __Float16x4_t);
__Float32x4_t __builtin_aarch64_fcadd270v4sf(__Float32x4_t, __Float32x4_t);
__Float16x8_t __builtin_aarch64_fcadd270v8hf(__Float16x8_t, __Float16x8_t);
__Float64x2_t __builtin_aarch64_fcadd90v2df(__Float64x2_t, __Float64x2_t);
__Float32x2_t __builtin_aarch64_fcadd90v2sf(__Float32x2_t, __Float32x2_t);
__Float16x4_t __builtin_aarch64_fcadd90v4hf(__Float16x4_t, __Float16x4_t);
__Float32x4_t __builtin_aarch64_fcadd90v4sf(__Float32x4_t, __Float32x4_t);
__Float16x8_t __builtin_aarch64_fcadd90v8hf(__Float16x8_t, __Float16x8_t);
__Float64x2_t __builtin_aarch64_fcmla0v2df(__Float64x2_t, __Float64x2_t, __Float64x2_t);
__Float32x2_t __builtin_aarch64_fcmla0v2sf(__Float32x2_t, __Float32x2_t, __Float32x2_t);
__Float16x4_t __builtin_aarch64_fcmla0v4hf(__Float16x4_t, __Float16x4_t, __Float16x4_t);
__Float32x4_t __builtin_aarch64_fcmla0v4sf(__Float32x4_t, __Float32x4_t, __Float32x4_t);
__Float16x8_t __builtin_aarch64_fcmla0v8hf(__Float16x8_t, __Float16x8_t, __Float16x8_t);
__Float64x2_t __builtin_aarch64_fcmla180v2df(__Float64x2_t, __Float64x2_t, __Float64x2_t);
__Float32x2_t __builtin_aarch64_fcmla180v2sf(__Float32x2_t, __Float32x2_t, __Float32x2_t);
__Float16x4_t __builtin_aarch64_fcmla180v4hf(__Float16x4_t, __Float16x4_t, __Float16x4_t);
__Float32x4_t __builtin_aarch64_fcmla180v4sf(__Float32x4_t, __Float32x4_t, __Float32x4_t);
__Float16x8_t __builtin_aarch64_fcmla180v8hf(__Float16x8_t, __Float16x8_t, __Float16x8_t);
__Float64x2_t __builtin_aarch64_fcmla270v2df(__Float64x2_t, __Float64x2_t, __Float64x2_t);
__Float32x2_t __builtin_aarch64_fcmla270v2sf(__Float32x2_t, __Float32x2_t, __Float32x2_t);
__Float16x4_t __builtin_aarch64_fcmla270v4hf(__Float16x4_t, __Float16x4_t, __Float16x4_t);
__Float32x4_t __builtin_aarch64_fcmla270v4sf(__Float32x4_t, __Float32x4_t, __Float32x4_t);
__Float16x8_t __builtin_aarch64_fcmla270v8hf(__Float16x8_t, __Float16x8_t, __Float16x8_t);
__Float64x2_t __builtin_aarch64_fcmla90v2df(__Float64x2_t, __Float64x2_t, __Float64x2_t);
__Float32x2_t __builtin_aarch64_fcmla90v2sf(__Float32x2_t, __Float32x2_t, __Float32x2_t);
__Float16x4_t __builtin_aarch64_fcmla90v4hf(__Float16x4_t, __Float16x4_t, __Float16x4_t);
__Float32x4_t __builtin_aarch64_fcmla90v4sf(__Float32x4_t, __Float32x4_t, __Float32x4_t);
__Float16x8_t __builtin_aarch64_fcmla90v8hf(__Float16x8_t, __Float16x8_t, __Float16x8_t);
__Float32x2_t __builtin_aarch64_fcmla_lane0v2sf(__Float32x2_t, __Float32x2_t, __Float32x2_t, int);
__Float16x4_t __builtin_aarch64_fcmla_lane0v4hf(__Float16x4_t, __Float16x4_t, __Float16x4_t, int);
__Float32x4_t __builtin_aarch64_fcmla_lane0v4sf(__Float32x4_t, __Float32x4_t, __Float32x4_t, int);
__Float16x8_t __builtin_aarch64_fcmla_lane0v8hf(__Float16x8_t, __Float16x8_t, __Float16x8_t, int);
__Float32x2_t __builtin_aarch64_fcmla_lane180v2sf(__Float32x2_t, __Float32x2_t, __Float32x2_t, int);
__Float16x4_t __builtin_aarch64_fcmla_lane180v4hf(__Float16x4_t, __Float16x4_t, __Float16x4_t, int);
__Float32x4_t __builtin_aarch64_fcmla_lane180v4sf(__Float32x4_t, __Float32x4_t, __Float32x4_t, int);
__Float16x8_t __builtin_aarch64_fcmla_lane180v8hf(__Float16x8_t, __Float16x8_t, __Float16x8_t, int);
__Float32x2_t __builtin_aarch64_fcmla_lane270v2sf(__Float32x2_t, __Float32x2_t, __Float32x2_t, int);
__Float16x4_t __builtin_aarch64_fcmla_lane270v4hf(__Float16x4_t, __Float16x4_t, __Float16x4_t, int);
__Float32x4_t __builtin_aarch64_fcmla_lane270v4sf(__Float32x4_t, __Float32x4_t, __Float32x4_t, int);
__Float16x8_t __builtin_aarch64_fcmla_lane270v8hf(__Float16x8_t, __Float16x8_t, __Float16x8_t, int);
__Float32x2_t __builtin_aarch64_fcmla_lane90v2sf(__Float32x2_t, __Float32x2_t, __Float32x2_t, int);
__Float16x4_t __builtin_aarch64_fcmla_lane90v4hf(__Float16x4_t, __Float16x4_t, __Float16x4_t, int);
__Float32x4_t __builtin_aarch64_fcmla_lane90v4sf(__Float32x4_t, __Float32x4_t, __Float32x4_t, int);
__Float16x8_t __builtin_aarch64_fcmla_lane90v8hf(__Float16x8_t, __Float16x8_t, __Float16x8_t, int);
__Float32x2_t __builtin_aarch64_fcmla_laneq0v2sf(__Float32x2_t, __Float32x2_t, __Float32x4_t, unsigned int);
__Float16x4_t __builtin_aarch64_fcmla_laneq0v4hf(__Float16x4_t, __Float16x4_t, __Float16x8_t, unsigned int);
__Float32x2_t __builtin_aarch64_fcmla_laneq180v2sf(__Float32x2_t, __Float32x2_t, __Float32x4_t, unsigned int);
__Float16x4_t __builtin_aarch64_fcmla_laneq180v4hf(__Float16x4_t, __Float16x4_t, __Float16x8_t, unsigned int);
__Float32x2_t __builtin_aarch64_fcmla_laneq270v2sf(__Float32x2_t, __Float32x2_t, __Float32x4_t, unsigned int);
__Float16x4_t __builtin_aarch64_fcmla_laneq270v4hf(__Float16x4_t, __Float16x4_t, __Float16x8_t, unsigned int);
__Float32x2_t __builtin_aarch64_fcmla_laneq90v2sf(__Float32x2_t, __Float32x2_t, __Float32x4_t, unsigned int);
__Float16x4_t __builtin_aarch64_fcmla_laneq90v4hf(__Float16x4_t, __Float16x4_t, __Float16x8_t, unsigned int);
__Float32x4_t __builtin_aarch64_fcmlaq_lane0v4sf(__Float32x4_t, __Float32x4_t, __Float32x2_t, int);
__Float16x8_t __builtin_aarch64_fcmlaq_lane0v8hf(__Float16x8_t, __Float16x8_t, __Float16x4_t, int);
__Float32x4_t __builtin_aarch64_fcmlaq_lane180v4sf(__Float32x4_t, __Float32x4_t, __Float32x2_t, int);
__Float16x8_t __builtin_aarch64_fcmlaq_lane180v8hf(__Float16x8_t, __Float16x8_t, __Float16x4_t, int);
__Float32x4_t __builtin_aarch64_fcmlaq_lane270v4sf(__Float32x4_t, __Float32x4_t, __Float32x2_t, int);
__Float16x8_t __builtin_aarch64_fcmlaq_lane270v8hf(__Float16x8_t, __Float16x8_t, __Float16x4_t, int);
__Float32x4_t __builtin_aarch64_fcmlaq_lane90v4sf(__Float32x4_t, __Float32x4_t, __Float32x2_t, int);
__Float16x8_t __builtin_aarch64_fcmlaq_lane90v8hf(__Float16x8_t, __Float16x8_t, __Float16x4_t, int);
long int __builtin_aarch64_fcvtzsdf(double, int);
short int __builtin_aarch64_fcvtzshf(__fp16, int);
long int __builtin_aarch64_fcvtzshfdi(__fp16, int);
int __builtin_aarch64_fcvtzshfsi(__fp16, int);
int __builtin_aarch64_fcvtzssf(float, int);
__Int64x2_t __builtin_aarch64_fcvtzsv2df(__Float64x2_t, int);
__Int32x2_t __builtin_aarch64_fcvtzsv2sf(__Float32x2_t, int);
__Int16x4_t __builtin_aarch64_fcvtzsv4hf(__Float16x4_t, int);
__Int32x4_t __builtin_aarch64_fcvtzsv4sf(__Float32x4_t, int);
__Int16x8_t __builtin_aarch64_fcvtzsv8hf(__Float16x8_t, int);
long unsigned int __builtin_aarch64_fcvtzudf_uss(double, int);
long unsigned int __builtin_aarch64_fcvtzuhfdi_uss(__fp16, int);
unsigned int __builtin_aarch64_fcvtzuhfsi_uss(__fp16, int);
short unsigned int __builtin_aarch64_fcvtzuhf_uss(__fp16, int);
unsigned int __builtin_aarch64_fcvtzusf_uss(float, int);
__Uint64x2_t __builtin_aarch64_fcvtzuv2df_uss(__Float64x2_t, int);
__Uint32x2_t __builtin_aarch64_fcvtzuv2sf_uss(__Float32x2_t, int);
__Uint16x4_t __builtin_aarch64_fcvtzuv4hf_uss(__Float16x4_t, int);
__Uint32x4_t __builtin_aarch64_fcvtzuv4sf_uss(__Float32x4_t, int);
__Uint16x8_t __builtin_aarch64_fcvtzuv8hf_uss(__Float16x8_t, int);
long int __builtin_aarch64_fix_trunchfdi(__fp16);
short int __builtin_aarch64_fix_trunchfhi(__fp16);
int __builtin_aarch64_fix_trunchfsi(__fp16);
long unsigned int __builtin_aarch64_fixuns_trunchfdi_us(__fp16);
short unsigned int __builtin_aarch64_fixuns_trunchfhi_us(__fp16);
unsigned int __builtin_aarch64_fixuns_trunchfsi_us(__fp16);
__fp16 __builtin_aarch64_floatdihf(long int);
__fp16 __builtin_aarch64_floathihf(short int);
__fp16 __builtin_aarch64_floatsihf(int);
__fp16 __builtin_aarch64_floatunsdihf_us(long int);
__fp16 __builtin_aarch64_floatunshihf_us(short int);
__fp16 __builtin_aarch64_floatunssihf_us(int);
__Float16x4_t __builtin_aarch64_floatunsv4hiv4hf(__Int16x4_t);
__Float16x8_t __builtin_aarch64_floatunsv8hiv8hf(__Int16x8_t);
__Float16x4_t __builtin_aarch64_floatv4hiv4hf(__Int16x4_t);
__Float16x8_t __builtin_aarch64_floatv8hiv8hf(__Int16x8_t);
__fp16 __builtin_aarch64_floorhf(__fp16);
__Float16x4_t __builtin_aarch64_floorv4hf(__Float16x4_t);
__Float16x8_t __builtin_aarch64_floorv8hf(__Float16x8_t);
__fp16 __builtin_aarch64_fmahf(__fp16, __fp16, __fp16);
__Float16x4_t __builtin_aarch64_fmav4hf(__Float16x4_t, __Float16x4_t, __Float16x4_t);
__Float16x8_t __builtin_aarch64_fmav8hf(__Float16x8_t, __Float16x8_t, __Float16x8_t);
double __builtin_aarch64_fmaxdf(double, double);
__fp16 __builtin_aarch64_fmaxhf(__fp16, __fp16);
__Float64x2_t __builtin_aarch64_fmaxv2df(__Float64x2_t, __Float64x2_t);
__Float32x2_t __builtin_aarch64_fmaxv2sf(__Float32x2_t, __Float32x2_t);
__Float16x4_t __builtin_aarch64_fmaxv4hf(__Float16x4_t, __Float16x4_t);
__Float32x4_t __builtin_aarch64_fmaxv4sf(__Float32x4_t, __Float32x4_t);
__Float16x8_t __builtin_aarch64_fmaxv8hf(__Float16x8_t, __Float16x8_t);
double __builtin_aarch64_fmindf(double, double);
__fp16 __builtin_aarch64_fminhf(__fp16, __fp16);
__Float64x2_t __builtin_aarch64_fminv2df(__Float64x2_t, __Float64x2_t);
__Float32x2_t __builtin_aarch64_fminv2sf(__Float32x2_t, __Float32x2_t);
__Float16x4_t __builtin_aarch64_fminv4hf(__Float16x4_t, __Float16x4_t);
__Float32x4_t __builtin_aarch64_fminv4sf(__Float32x4_t, __Float32x4_t);
__Float16x8_t __builtin_aarch64_fminv8hf(__Float16x8_t, __Float16x8_t);
__Float32x2_t __builtin_aarch64_fmlal_highv2sf(__Float32x2_t, __Float16x4_t, __Float16x4_t);
__Float32x2_t __builtin_aarch64_fmlal_lane_highv2sf(__Float32x2_t, __Float16x4_t, __Float16x4_t, int);
__Float32x2_t __builtin_aarch64_fmlal_lane_lowv2sf(__Float32x2_t, __Float16x4_t, __Float16x4_t, int);
__Float32x2_t __builtin_aarch64_fmlal_laneq_highv2sf(__Float32x2_t, __Float16x4_t, __Float16x8_t, int);
__Float32x2_t __builtin_aarch64_fmlal_laneq_lowv2sf(__Float32x2_t, __Float16x4_t, __Float16x8_t, int);
__Float32x2_t __builtin_aarch64_fmlal_lowv2sf(__Float32x2_t, __Float16x4_t, __Float16x4_t);
__Float32x4_t __builtin_aarch64_fmlalq_highv4sf(__Float32x4_t, __Float16x8_t, __Float16x8_t);
__Float32x4_t __builtin_aarch64_fmlalq_lane_highv4sf(__Float32x4_t, __Float16x8_t, __Float16x4_t, int);
__Float32x4_t __builtin_aarch64_fmlalq_lane_lowv4sf(__Float32x4_t, __Float16x8_t, __Float16x4_t, int);
__Float32x4_t __builtin_aarch64_fmlalq_laneq_highv4sf(__Float32x4_t, __Float16x8_t, __Float16x8_t, int);
__Float32x4_t __builtin_aarch64_fmlalq_laneq_lowv4sf(__Float32x4_t, __Float16x8_t, __Float16x8_t, int);
__Float32x4_t __builtin_aarch64_fmlalq_lowv4sf(__Float32x4_t, __Float16x8_t, __Float16x8_t);
__Float32x2_t __builtin_aarch64_fmlsl_highv2sf(__Float32x2_t, __Float16x4_t, __Float16x4_t);
__Float32x2_t __builtin_aarch64_fmlsl_lane_highv2sf(__Float32x2_t, __Float16x4_t, __Float16x4_t, int);
__Float32x2_t __builtin_aarch64_fmlsl_lane_lowv2sf(__Float32x2_t, __Float16x4_t, __Float16x4_t, int);
__Float32x2_t __builtin_aarch64_fmlsl_laneq_highv2sf(__Float32x2_t, __Float16x4_t, __Float16x8_t, int);
__Float32x2_t __builtin_aarch64_fmlsl_laneq_lowv2sf(__Float32x2_t, __Float16x4_t, __Float16x8_t, int);
__Float32x2_t __builtin_aarch64_fmlsl_lowv2sf(__Float32x2_t, __Float16x4_t, __Float16x4_t);
__Float32x4_t __builtin_aarch64_fmlslq_highv4sf(__Float32x4_t, __Float16x8_t, __Float16x8_t);
__Float32x4_t __builtin_aarch64_fmlslq_lane_highv4sf(__Float32x4_t, __Float16x8_t, __Float16x4_t, int);
__Float32x4_t __builtin_aarch64_fmlslq_lane_lowv4sf(__Float32x4_t, __Float16x8_t, __Float16x4_t, int);
__Float32x4_t __builtin_aarch64_fmlslq_laneq_highv4sf(__Float32x4_t, __Float16x8_t, __Float16x8_t, int);
__Float32x4_t __builtin_aarch64_fmlslq_laneq_lowv4sf(__Float32x4_t, __Float16x8_t, __Float16x8_t, int);
__Float32x4_t __builtin_aarch64_fmlslq_lowv4sf(__Float32x4_t, __Float16x8_t, __Float16x8_t);
double __builtin_aarch64_fmulxdf(double, double);
__fp16 __builtin_aarch64_fmulxhf(__fp16, __fp16);
float __builtin_aarch64_fmulxsf(float, float);
__Float64x2_t __builtin_aarch64_fmulxv2df(__Float64x2_t, __Float64x2_t);
__Float32x2_t __builtin_aarch64_fmulxv2sf(__Float32x2_t, __Float32x2_t);
__Float16x4_t __builtin_aarch64_fmulxv4hf(__Float16x4_t, __Float16x4_t);
__Float32x4_t __builtin_aarch64_fmulxv4sf(__Float32x4_t, __Float32x4_t);
__Float16x8_t __builtin_aarch64_fmulxv8hf(__Float16x8_t, __Float16x8_t);
__fp16 __builtin_aarch64_fnmahf(__fp16, __fp16, __fp16);
__Float16x4_t __builtin_aarch64_fnmav4hf(__Float16x4_t, __Float16x4_t, __Float16x4_t);
__Float16x8_t __builtin_aarch64_fnmav8hf(__Float16x8_t, __Float16x8_t, __Float16x8_t);
double __builtin_aarch64_frecpedf(double);
__fp16 __builtin_aarch64_frecpehf(__fp16);
float __builtin_aarch64_frecpesf(float);
__Float16x4_t __builtin_aarch64_frecpev4hf(__Float16x4_t);
__Float16x8_t __builtin_aarch64_frecpev8hf(__Float16x8_t);
double __builtin_aarch64_frecpsdf(double, double);
__fp16 __builtin_aarch64_frecpshf(__fp16, __fp16);
float __builtin_aarch64_frecpssf(float, float);
__Float16x4_t __builtin_aarch64_frecpsv4hf(__Float16x4_t, __Float16x4_t);
__Float16x8_t __builtin_aarch64_frecpsv8hf(__Float16x8_t, __Float16x8_t);
double __builtin_aarch64_frecpxdf(double);
__fp16 __builtin_aarch64_frecpxhf(__fp16);
float __builtin_aarch64_frecpxsf(float);
double __builtin_aarch64_frintndf(double);
__fp16 __builtin_aarch64_frintnhf(__fp16);
__Float16x4_t __builtin_aarch64_frintnv4hf(__Float16x4_t);
__Float16x8_t __builtin_aarch64_frintnv8hf(__Float16x8_t);
__Poly64_t __builtin_aarch64_get_dregcidi_pss(__builtin_aarch64_simd_ci, int);
__Poly64_t __builtin_aarch64_get_dregoidi_pss(__builtin_aarch64_simd_oi, int);
__Poly64_t __builtin_aarch64_get_dregxidi_pss(__builtin_aarch64_simd_xi, int);
__Poly64x2_t __builtin_aarch64_get_qregciv2di_pss(__builtin_aarch64_simd_ci, int);
__Poly64x2_t __builtin_aarch64_get_qregoiv2di_pss(__builtin_aarch64_simd_oi, int);
__Poly64x2_t __builtin_aarch64_get_qregxiv2di_pss(__builtin_aarch64_simd_xi, int);
__Uint16x4_t __builtin_aarch64_lbtruncuv4hfv4hi_us(__Float16x4_t);
__Uint16x8_t __builtin_aarch64_lbtruncuv8hfv8hi_us(__Float16x8_t);
__Int16x4_t __builtin_aarch64_lbtruncv4hfv4hi(__Float16x4_t);
__Int16x8_t __builtin_aarch64_lbtruncv8hfv8hi(__Float16x8_t);
long int __builtin_aarch64_lceilhfdi(__fp16);
short int __builtin_aarch64_lceilhfhi(__fp16);
int __builtin_aarch64_lceilhfsi(__fp16);
long unsigned int __builtin_aarch64_lceiludfdi_us(double);
long unsigned int __builtin_aarch64_lceiluhfdi_us(__fp16);
short unsigned int __builtin_aarch64_lceiluhfhi_us(__fp16);
unsigned int __builtin_aarch64_lceiluhfsi_us(__fp16);
unsigned int __builtin_aarch64_lceilusfsi_us(float);
__Uint16x4_t __builtin_aarch64_lceiluv4hfv4hi_us(__Float16x4_t);
__Uint16x8_t __builtin_aarch64_lceiluv8hfv8hi_us(__Float16x8_t);
__Int16x4_t __builtin_aarch64_lceilv4hfv4hi(__Float16x4_t);
__Int16x8_t __builtin_aarch64_lceilv8hfv8hi(__Float16x8_t);
__builtin_aarch64_simd_oi __builtin_aarch64_ld1x2df(const double*);
__builtin_aarch64_simd_oi __builtin_aarch64_ld1x2di(const long int*);
__builtin_aarch64_simd_oi __builtin_aarch64_ld1x2v16qi(const signed char*);
__builtin_aarch64_simd_oi __builtin_aarch64_ld1x2v2df(const double*);
__builtin_aarch64_simd_oi __builtin_aarch64_ld1x2v2di(const long int*);
__builtin_aarch64_simd_oi __builtin_aarch64_ld1x2v2sf(const float*);
__builtin_aarch64_simd_oi __builtin_aarch64_ld1x2v2si(const int*);
__builtin_aarch64_simd_oi __builtin_aarch64_ld1x2v4hf(const __fp16*);
__builtin_aarch64_simd_oi __builtin_aarch64_ld1x2v4hi(const short int*);
__builtin_aarch64_simd_oi __builtin_aarch64_ld1x2v4sf(const float*);
__builtin_aarch64_simd_oi __builtin_aarch64_ld1x2v4si(const int*);
__builtin_aarch64_simd_oi __builtin_aarch64_ld1x2v8hf(const __fp16*);
__builtin_aarch64_simd_oi __builtin_aarch64_ld1x2v8hi(const short int*);
__builtin_aarch64_simd_oi __builtin_aarch64_ld1x2v8qi(const signed char*);
__builtin_aarch64_simd_ci __builtin_aarch64_ld1x3df(const double*);
__builtin_aarch64_simd_ci __builtin_aarch64_ld1x3di(const long int*);
__builtin_aarch64_simd_ci __builtin_aarch64_ld1x3v16qi(const signed char*);
__builtin_aarch64_simd_ci __builtin_aarch64_ld1x3v2df(const double*);
__builtin_aarch64_simd_ci __builtin_aarch64_ld1x3v2di(const long int*);
__builtin_aarch64_simd_ci __builtin_aarch64_ld1x3v2sf(const float*);
__builtin_aarch64_simd_ci __builtin_aarch64_ld1x3v2si(const int*);
__builtin_aarch64_simd_ci __builtin_aarch64_ld1x3v4hf(const __fp16*);
__builtin_aarch64_simd_ci __builtin_aarch64_ld1x3v4hi(const short int*);
__builtin_aarch64_simd_ci __builtin_aarch64_ld1x3v4sf(const float*);
__builtin_aarch64_simd_ci __builtin_aarch64_ld1x3v4si(const int*);
__builtin_aarch64_simd_ci __builtin_aarch64_ld1x3v8hf(const __fp16*);
__builtin_aarch64_simd_ci __builtin_aarch64_ld1x3v8hi(const short int*);
__builtin_aarch64_simd_ci __builtin_aarch64_ld1x3v8qi(const signed char*);
__builtin_aarch64_simd_oi __builtin_aarch64_ld2df(const double*);
__builtin_aarch64_simd_oi __builtin_aarch64_ld2di(const long int*);
__builtin_aarch64_simd_oi __builtin_aarch64_ld2rdf(const double*);
__builtin_aarch64_simd_oi __builtin_aarch64_ld2rdi(const long int*);
__builtin_aarch64_simd_oi __builtin_aarch64_ld2rv16qi(const signed char*);
__builtin_aarch64_simd_oi __builtin_aarch64_ld2rv2df(const double*);
__builtin_aarch64_simd_oi __builtin_aarch64_ld2rv2di(const long int*);
__builtin_aarch64_simd_oi __builtin_aarch64_ld2rv2sf(const float*);
__builtin_aarch64_simd_oi __builtin_aarch64_ld2rv2si(const int*);
__builtin_aarch64_simd_oi __builtin_aarch64_ld2rv4hf(const __fp16*);
__builtin_aarch64_simd_oi __builtin_aarch64_ld2rv4hi(const short int*);
__builtin_aarch64_simd_oi __builtin_aarch64_ld2rv4sf(const float*);
__builtin_aarch64_simd_oi __builtin_aarch64_ld2rv4si(const int*);
__builtin_aarch64_simd_oi __builtin_aarch64_ld2rv8hf(const __fp16*);
__builtin_aarch64_simd_oi __builtin_aarch64_ld2rv8hi(const short int*);
__builtin_aarch64_simd_oi __builtin_aarch64_ld2rv8qi(const signed char*);
__builtin_aarch64_simd_oi __builtin_aarch64_ld2v16qi(const signed char*);
__builtin_aarch64_simd_oi __builtin_aarch64_ld2v2df(const double*);
__builtin_aarch64_simd_oi __builtin_aarch64_ld2v2di(const long int*);
__builtin_aarch64_simd_oi __builtin_aarch64_ld2v2sf(const float*);
__builtin_aarch64_simd_oi __builtin_aarch64_ld2v2si(const int*);
__builtin_aarch64_simd_oi __builtin_aarch64_ld2v4hf(const __fp16*);
__builtin_aarch64_simd_oi __builtin_aarch64_ld2v4hi(const short int*);
__builtin_aarch64_simd_oi __builtin_aarch64_ld2v4sf(const float*);
__builtin_aarch64_simd_oi __builtin_aarch64_ld2v4si(const int*);
__builtin_aarch64_simd_oi __builtin_aarch64_ld2v8hf(const __fp16*);
__builtin_aarch64_simd_oi __builtin_aarch64_ld2v8hi(const short int*);
__builtin_aarch64_simd_oi __builtin_aarch64_ld2v8qi(const signed char*);
__builtin_aarch64_simd_ci __builtin_aarch64_ld3df(const double*);
__builtin_aarch64_simd_ci __builtin_aarch64_ld3rdf(const double*);
__builtin_aarch64_simd_ci __builtin_aarch64_ld3rdi(const long int*);
__builtin_aarch64_simd_ci __builtin_aarch64_ld3rv16qi(const signed char*);
__builtin_aarch64_simd_ci __builtin_aarch64_ld3rv2df(const double*);
__builtin_aarch64_simd_ci __builtin_aarch64_ld3rv2di(const long int*);
__builtin_aarch64_simd_ci __builtin_aarch64_ld3rv2sf(const float*);
__builtin_aarch64_simd_ci __builtin_aarch64_ld3rv2si(const int*);
__builtin_aarch64_simd_ci __builtin_aarch64_ld3rv4hf(const __fp16*);
__builtin_aarch64_simd_ci __builtin_aarch64_ld3rv4hi(const short int*);
__builtin_aarch64_simd_ci __builtin_aarch64_ld3rv4sf(const float*);
__builtin_aarch64_simd_ci __builtin_aarch64_ld3rv4si(const int*);
__builtin_aarch64_simd_ci __builtin_aarch64_ld3rv8hf(const __fp16*);
__builtin_aarch64_simd_ci __builtin_aarch64_ld3rv8hi(const short int*);
__builtin_aarch64_simd_ci __builtin_aarch64_ld3rv8qi(const signed char*);
__builtin_aarch64_simd_ci __builtin_aarch64_ld3v16qi(const signed char*);
__builtin_aarch64_simd_ci __builtin_aarch64_ld3v2df(const double*);
__builtin_aarch64_simd_ci __builtin_aarch64_ld3v2di(const long int*);
__builtin_aarch64_simd_ci __builtin_aarch64_ld3v2sf(const float*);
__builtin_aarch64_simd_ci __builtin_aarch64_ld3v2si(const int*);
__builtin_aarch64_simd_ci __builtin_aarch64_ld3v4hf(const __fp16*);
__builtin_aarch64_simd_ci __builtin_aarch64_ld3v4hi(const short int*);
__builtin_aarch64_simd_ci __builtin_aarch64_ld3v4sf(const float*);
__builtin_aarch64_simd_ci __builtin_aarch64_ld3v4si(const int*);
__builtin_aarch64_simd_ci __builtin_aarch64_ld3v8hf(const __fp16*);
__builtin_aarch64_simd_ci __builtin_aarch64_ld3v8hi(const short int*);
__builtin_aarch64_simd_ci __builtin_aarch64_ld3v8qi(const signed char*);
__builtin_aarch64_simd_xi __builtin_aarch64_ld4df(const double*);
__builtin_aarch64_simd_xi __builtin_aarch64_ld4di(const long int*);
__builtin_aarch64_simd_xi __builtin_aarch64_ld4rdf(const double*);
__builtin_aarch64_simd_xi __builtin_aarch64_ld4rdi(const long int*);
__builtin_aarch64_simd_xi __builtin_aarch64_ld4rv16qi(const signed char*);
__builtin_aarch64_simd_xi __builtin_aarch64_ld4rv2df(const double*);
__builtin_aarch64_simd_xi __builtin_aarch64_ld4rv2di(const long int*);
__builtin_aarch64_simd_xi __builtin_aarch64_ld4rv2sf(const float*);
__builtin_aarch64_simd_xi __builtin_aarch64_ld4rv2si(const int*);
__builtin_aarch64_simd_xi __builtin_aarch64_ld4rv4hf(const __fp16*);
__builtin_aarch64_simd_xi __builtin_aarch64_ld4rv4hi(const short int*);
__builtin_aarch64_simd_xi __builtin_aarch64_ld4rv4sf(const float*);
__builtin_aarch64_simd_xi __builtin_aarch64_ld4rv4si(const int*);
__builtin_aarch64_simd_xi __builtin_aarch64_ld4rv8hf(const __fp16*);
__builtin_aarch64_simd_xi __builtin_aarch64_ld4rv8hi(const short int*);
__builtin_aarch64_simd_xi __builtin_aarch64_ld4rv8qi(const signed char*);
__builtin_aarch64_simd_xi __builtin_aarch64_ld4v16qi(const signed char*);
__builtin_aarch64_simd_xi __builtin_aarch64_ld4v2df(const double*);
__builtin_aarch64_simd_xi __builtin_aarch64_ld4v2di(const long int*);
__builtin_aarch64_simd_xi __builtin_aarch64_ld4v2sf(const float*);
__builtin_aarch64_simd_xi __builtin_aarch64_ld4v2si(const int*);
__builtin_aarch64_simd_xi __builtin_aarch64_ld4v4hf(const __fp16*);
__builtin_aarch64_simd_xi __builtin_aarch64_ld4v4hi(const short int*);
__builtin_aarch64_simd_xi __builtin_aarch64_ld4v4sf(const float*);
__builtin_aarch64_simd_xi __builtin_aarch64_ld4v4si(const int*);
__builtin_aarch64_simd_xi __builtin_aarch64_ld4v8hf(const __fp16*);
__builtin_aarch64_simd_xi __builtin_aarch64_ld4v8hi(const short int*);
__builtin_aarch64_simd_xi __builtin_aarch64_ld4v8qi(const signed char*);
long int __builtin_aarch64_lfloorhfdi(__fp16);
short int __builtin_aarch64_lfloorhfhi(__fp16);
int __builtin_aarch64_lfloorhfsi(__fp16);
long unsigned int __builtin_aarch64_lfloorudfdi_us(double);
long unsigned int __builtin_aarch64_lflooruhfdi_us(__fp16);
short unsigned int __builtin_aarch64_lflooruhfhi_us(__fp16);
unsigned int __builtin_aarch64_lflooruhfsi_us(__fp16);
unsigned int __builtin_aarch64_lfloorusfsi_us(float);
__Uint16x4_t __builtin_aarch64_lflooruv4hfv4hi_us(__Float16x4_t);
__Uint16x8_t __builtin_aarch64_lflooruv8hfv8hi_us(__Float16x8_t);
__Int16x4_t __builtin_aarch64_lfloorv4hfv4hi(__Float16x4_t);
__Int16x8_t __builtin_aarch64_lfloorv8hfv8hi(__Float16x8_t);
long int __builtin_aarch64_lfrintndfdi(double);
long int __builtin_aarch64_lfrintnhfdi(__fp16);
short int __builtin_aarch64_lfrintnhfhi(__fp16);
int __builtin_aarch64_lfrintnhfsi(__fp16);
int __builtin_aarch64_lfrintnsfsi(float);
long unsigned int __builtin_aarch64_lfrintnudfdi_us(double);
long unsigned int __builtin_aarch64_lfrintnuhfdi_us(__fp16);
short unsigned int __builtin_aarch64_lfrintnuhfhi_us(__fp16);
unsigned int __builtin_aarch64_lfrintnuhfsi_us(__fp16);
unsigned int __builtin_aarch64_lfrintnusfsi_us(float);
__Uint16x4_t __builtin_aarch64_lfrintnuv4hfv4hi_us(__Float16x4_t);
__Uint16x8_t __builtin_aarch64_lfrintnuv8hfv8hi_us(__Float16x8_t);
__Int16x4_t __builtin_aarch64_lfrintnv4hfv4hi(__Float16x4_t);
__Int16x8_t __builtin_aarch64_lfrintnv8hfv8hi(__Float16x8_t);
long int __builtin_aarch64_lrounddfdi(double);
long int __builtin_aarch64_lroundhfdi(__fp16);
short int __builtin_aarch64_lroundhfhi(__fp16);
int __builtin_aarch64_lroundhfsi(__fp16);
int __builtin_aarch64_lroundsfsi(float);
long unsigned int __builtin_aarch64_lroundudfdi_us(double);
long unsigned int __builtin_aarch64_lrounduhfdi_us(__fp16);
short unsigned int __builtin_aarch64_lrounduhfhi_us(__fp16);
unsigned int __builtin_aarch64_lrounduhfsi_us(__fp16);
unsigned int __builtin_aarch64_lroundusfsi_us(float);
__Uint16x4_t __builtin_aarch64_lrounduv4hfv4hi_us(__Float16x4_t);
__Uint16x8_t __builtin_aarch64_lrounduv8hfv8hi_us(__Float16x8_t);
__Int16x4_t __builtin_aarch64_lroundv4hfv4hi(__Float16x4_t);
__Int16x8_t __builtin_aarch64_lroundv8hfv8hi(__Float16x8_t);
long unsigned int __builtin_aarch64_lshr_simddi_uus(long unsigned int, int);
__fp16 __builtin_aarch64_nearbyinthf(__fp16);
__Float16x4_t __builtin_aarch64_nearbyintv4hf(__Float16x4_t);
__Float16x8_t __builtin_aarch64_nearbyintv8hf(__Float16x8_t);
__fp16 __builtin_aarch64_neghf(__fp16);
__Int8x16_t __builtin_aarch64_qtbl3v16qi(__builtin_aarch64_simd_ci, __Int8x16_t);
__Int8x8_t __builtin_aarch64_qtbl3v8qi(__builtin_aarch64_simd_ci, __Int8x8_t);
__Int8x16_t __builtin_aarch64_qtbl4v16qi(__builtin_aarch64_simd_xi, __Int8x16_t);
__Int8x8_t __builtin_aarch64_qtbl4v8qi(__builtin_aarch64_simd_xi, __Int8x8_t);
__Int8x16_t __builtin_aarch64_qtbx3v16qi(__Int8x16_t, __builtin_aarch64_simd_ci, __Int8x16_t);
__Int8x8_t __builtin_aarch64_qtbx3v8qi(__Int8x8_t, __builtin_aarch64_simd_ci, __Int8x8_t);
__Int8x16_t __builtin_aarch64_qtbx4v16qi(__Int8x16_t, __builtin_aarch64_simd_xi, __Int8x16_t);
__Int8x8_t __builtin_aarch64_qtbx4v8qi(__Int8x8_t, __builtin_aarch64_simd_xi, __Int8x8_t);
__Uint64x2_t __builtin_aarch64_rax1qv2di_uuu(__Uint64x2_t, __Uint64x2_t);
signed char __builtin_aarch64_reduc_plus_scal_v16qi(__Int8x16_t);
double __builtin_aarch64_reduc_plus_scal_v2df(__Float64x2_t);
long int __builtin_aarch64_reduc_plus_scal_v2di(__Int64x2_t);
float __builtin_aarch64_reduc_plus_scal_v2sf(__Float32x2_t);
int __builtin_aarch64_reduc_plus_scal_v2si(__Int32x2_t);
short int __builtin_aarch64_reduc_plus_scal_v4hi(__Int16x4_t);
float __builtin_aarch64_reduc_plus_scal_v4sf(__Float32x4_t);
int __builtin_aarch64_reduc_plus_scal_v4si(__Int32x4_t);
short int __builtin_aarch64_reduc_plus_scal_v8hi(__Int16x8_t);
signed char __builtin_aarch64_reduc_plus_scal_v8qi(__Int8x8_t);
double __builtin_aarch64_reduc_smax_nan_scal_v2df(__Float64x2_t);
float __builtin_aarch64_reduc_smax_nan_scal_v2sf(__Float32x2_t);
__fp16 __builtin_aarch64_reduc_smax_nan_scal_v4hf(__Float16x4_t);
float __builtin_aarch64_reduc_smax_nan_scal_v4sf(__Float32x4_t);
__fp16 __builtin_aarch64_reduc_smax_nan_scal_v8hf(__Float16x8_t);
signed char __builtin_aarch64_reduc_smax_scal_v16qi(__Int8x16_t);
double __builtin_aarch64_reduc_smax_scal_v2df(__Float64x2_t);
float __builtin_aarch64_reduc_smax_scal_v2sf(__Float32x2_t);
int __builtin_aarch64_reduc_smax_scal_v2si(__Int32x2_t);
__fp16 __builtin_aarch64_reduc_smax_scal_v4hf(__Float16x4_t);
short int __builtin_aarch64_reduc_smax_scal_v4hi(__Int16x4_t);
float __builtin_aarch64_reduc_smax_scal_v4sf(__Float32x4_t);
int __builtin_aarch64_reduc_smax_scal_v4si(__Int32x4_t);
__fp16 __builtin_aarch64_reduc_smax_scal_v8hf(__Float16x8_t);
short int __builtin_aarch64_reduc_smax_scal_v8hi(__Int16x8_t);
signed char __builtin_aarch64_reduc_smax_scal_v8qi(__Int8x8_t);
double __builtin_aarch64_reduc_smin_nan_scal_v2df(__Float64x2_t);
float __builtin_aarch64_reduc_smin_nan_scal_v2sf(__Float32x2_t);
__fp16 __builtin_aarch64_reduc_smin_nan_scal_v4hf(__Float16x4_t);
float __builtin_aarch64_reduc_smin_nan_scal_v4sf(__Float32x4_t);
__fp16 __builtin_aarch64_reduc_smin_nan_scal_v8hf(__Float16x8_t);
signed char __builtin_aarch64_reduc_smin_scal_v16qi(__Int8x16_t);
double __builtin_aarch64_reduc_smin_scal_v2df(__Float64x2_t);
float __builtin_aarch64_reduc_smin_scal_v2sf(__Float32x2_t);
int __builtin_aarch64_reduc_smin_scal_v2si(__Int32x2_t);
__fp16 __builtin_aarch64_reduc_smin_scal_v4hf(__Float16x4_t);
short int __builtin_aarch64_reduc_smin_scal_v4hi(__Int16x4_t);
float __builtin_aarch64_reduc_smin_scal_v4sf(__Float32x4_t);
int __builtin_aarch64_reduc_smin_scal_v4si(__Int32x4_t);
__fp16 __builtin_aarch64_reduc_smin_scal_v8hf(__Float16x8_t);
short int __builtin_aarch64_reduc_smin_scal_v8hi(__Int16x8_t);
signed char __builtin_aarch64_reduc_smin_scal_v8qi(__Int8x8_t);
unsigned char __builtin_aarch64_reduc_umax_scal_v16qi_uu(__Uint8x16_t);
unsigned int __builtin_aarch64_reduc_umax_scal_v2si_uu(__Uint32x2_t);
short unsigned int __builtin_aarch64_reduc_umax_scal_v4hi_uu(__Uint16x4_t);
unsigned int __builtin_aarch64_reduc_umax_scal_v4si_uu(__Uint32x4_t);
short unsigned int __builtin_aarch64_reduc_umax_scal_v8hi_uu(__Uint16x8_t);
unsigned char __builtin_aarch64_reduc_umax_scal_v8qi_uu(__Uint8x8_t);
unsigned char __builtin_aarch64_reduc_umin_scal_v16qi_uu(__Uint8x16_t);
unsigned int __builtin_aarch64_reduc_umin_scal_v2si_uu(__Uint32x2_t);
short unsigned int __builtin_aarch64_reduc_umin_scal_v4hi_uu(__Uint16x4_t);
unsigned int __builtin_aarch64_reduc_umin_scal_v4si_uu(__Uint32x4_t);
short unsigned int __builtin_aarch64_reduc_umin_scal_v8hi_uu(__Uint16x8_t);
unsigned char __builtin_aarch64_reduc_umin_scal_v8qi_uu(__Uint8x8_t);
__fp16 __builtin_aarch64_rinthf(__fp16);
__Float16x4_t __builtin_aarch64_rintv4hf(__Float16x4_t);
__Float16x8_t __builtin_aarch64_rintv8hf(__Float16x8_t);
__fp16 __builtin_aarch64_roundhf(__fp16);
__Float16x4_t __builtin_aarch64_roundv4hf(__Float16x4_t);
__Float16x8_t __builtin_aarch64_roundv8hf(__Float16x8_t);
double __builtin_aarch64_rsqrtedf(double);
__fp16 __builtin_aarch64_rsqrtehf(__fp16);
float __builtin_aarch64_rsqrtesf(float);
__Float64x2_t __builtin_aarch64_rsqrtev2df(__Float64x2_t);
__Float32x2_t __builtin_aarch64_rsqrtev2sf(__Float32x2_t);
__Float16x4_t __builtin_aarch64_rsqrtev4hf(__Float16x4_t);
__Float32x4_t __builtin_aarch64_rsqrtev4sf(__Float32x4_t);
__Float16x8_t __builtin_aarch64_rsqrtev8hf(__Float16x8_t);
double __builtin_aarch64_rsqrtsdf(double, double);
__fp16 __builtin_aarch64_rsqrtshf(__fp16, __fp16);
float __builtin_aarch64_rsqrtssf(float, float);
__Float64x2_t __builtin_aarch64_rsqrtsv2df(__Float64x2_t, __Float64x2_t);
__Float32x2_t __builtin_aarch64_rsqrtsv2sf(__Float32x2_t, __Float32x2_t);
__Float16x4_t __builtin_aarch64_rsqrtsv4hf(__Float16x4_t, __Float16x4_t);
__Float32x4_t __builtin_aarch64_rsqrtsv4sf(__Float32x4_t, __Float32x4_t);
__Float16x8_t __builtin_aarch64_rsqrtsv8hf(__Float16x8_t, __Float16x8_t);
double __builtin_aarch64_scvtfdi(long int, int);
__fp16 __builtin_aarch64_scvtfdihf(long int, int);
__fp16 __builtin_aarch64_scvtfhi(short int, int);
float __builtin_aarch64_scvtfsi(int, int);
__fp16 __builtin_aarch64_scvtfsihf(int, int);
__Float64x2_t __builtin_aarch64_scvtfv2di(__Int64x2_t, int);
__Float32x2_t __builtin_aarch64_scvtfv2si(__Int32x2_t, int);
__Float16x4_t __builtin_aarch64_scvtfv4hi(__Int16x4_t, int);
__Float32x4_t __builtin_aarch64_scvtfv4si(__Int32x4_t, int);
__Float16x8_t __builtin_aarch64_scvtfv8hi(__Int16x8_t, int);
__Int32x4_t __builtin_aarch64_sdot_laneqv16qi(__Int32x4_t, __Int8x16_t, __Int8x16_t, int);
__Int32x2_t __builtin_aarch64_sdot_laneqv8qi(__Int32x2_t, __Int8x8_t, __Int8x16_t, int);
__Int32x4_t __builtin_aarch64_sdot_lanev16qi(__Int32x4_t, __Int8x16_t, __Int8x8_t, int);
__Int32x2_t __builtin_aarch64_sdot_lanev8qi(__Int32x2_t, __Int8x8_t, __Int8x8_t, int);
__Int32x4_t __builtin_aarch64_sdotv16qi(__Int32x4_t, __Int8x16_t, __Int8x16_t);
__Int32x2_t __builtin_aarch64_sdotv8qi(__Int32x2_t, __Int8x8_t, __Int8x8_t);
__builtin_aarch64_simd_ci __builtin_aarch64_set_qregciv2di_ssps(__builtin_aarch64_simd_ci, __Poly64x2_t, int);
__builtin_aarch64_simd_oi __builtin_aarch64_set_qregoiv2di_ssps(__builtin_aarch64_simd_oi, __Poly64x2_t, int);
__builtin_aarch64_simd_xi __builtin_aarch64_set_qregxiv2di_ssps(__builtin_aarch64_simd_xi, __Poly64x2_t, int);
double __builtin_aarch64_simd_bsldf_suss(long unsigned int, double, double);
__Poly64_t __builtin_aarch64_simd_bsldi_pupp(long unsigned int, __Poly64_t, __Poly64_t);
long int __builtin_aarch64_simd_bsldi_suss(long unsigned int, long int, long int);
long unsigned int __builtin_aarch64_simd_bsldi_uuuu(long unsigned int, long unsigned int, long unsigned int);
__Poly64x2_t __builtin_aarch64_simd_bslv2di_pupp(__Uint64x2_t, __Poly64x2_t, __Poly64x2_t);
__Float16x4_t __builtin_aarch64_simd_bslv4hf_suss(__Uint16x4_t, __Float16x4_t, __Float16x4_t);
__Float16x8_t __builtin_aarch64_simd_bslv8hf_suss(__Uint16x8_t, __Float16x8_t, __Float16x8_t);
__Uint32x4_t __builtin_aarch64_sm3partw1qv4si_uuuu(__Uint32x4_t, __Uint32x4_t, __Uint32x4_t);
__Uint32x4_t __builtin_aarch64_sm3partw2qv4si_uuuu(__Uint32x4_t, __Uint32x4_t, __Uint32x4_t);
__Uint32x4_t __builtin_aarch64_sm3ss1qv4si_uuuu(__Uint32x4_t, __Uint32x4_t, __Uint32x4_t);
__Uint32x4_t __builtin_aarch64_sm3tt1aqv4si_uuuus(__Uint32x4_t, __Uint32x4_t, __Uint32x4_t, int);
__Uint32x4_t __builtin_aarch64_sm3tt1bqv4si_uuuus(__Uint32x4_t, __Uint32x4_t, __Uint32x4_t, int);
__Uint32x4_t __builtin_aarch64_sm3tt2aqv4si_uuuus(__Uint32x4_t, __Uint32x4_t, __Uint32x4_t, int);
__Uint32x4_t __builtin_aarch64_sm3tt2bqv4si_uuuus(__Uint32x4_t, __Uint32x4_t, __Uint32x4_t, int);
__Uint32x4_t __builtin_aarch64_sm4ekeyqv4si_uuu(__Uint32x4_t, __Uint32x4_t);
__Uint32x4_t __builtin_aarch64_sm4eqv4si_uuu(__Uint32x4_t, __Uint32x4_t);
double __builtin_aarch64_smax_nandf(double, double);
__Float16x4_t __builtin_aarch64_smax_nanpv4hf(__Float16x4_t, __Float16x4_t);
__Float16x8_t __builtin_aarch64_smax_nanpv8hf(__Float16x8_t, __Float16x8_t);
__Float16x4_t __builtin_aarch64_smax_nanv4hf(__Float16x4_t, __Float16x4_t);
__Float16x8_t __builtin_aarch64_smax_nanv8hf(__Float16x8_t, __Float16x8_t);
__Float16x4_t __builtin_aarch64_smaxpv4hf(__Float16x4_t, __Float16x4_t);
__Float16x8_t __builtin_aarch64_smaxpv8hf(__Float16x8_t, __Float16x8_t);
double __builtin_aarch64_smin_nandf(double, double);
__Float16x4_t __builtin_aarch64_smin_nanpv4hf(__Float16x4_t, __Float16x4_t);
__Float16x8_t __builtin_aarch64_smin_nanpv8hf(__Float16x8_t, __Float16x8_t);
__Float16x4_t __builtin_aarch64_smin_nanv4hf(__Float16x4_t, __Float16x4_t);
__Float16x8_t __builtin_aarch64_smin_nanv8hf(__Float16x8_t, __Float16x8_t);
__Float16x4_t __builtin_aarch64_sminpv4hf(__Float16x4_t, __Float16x4_t);
__Float16x8_t __builtin_aarch64_sminpv8hf(__Float16x8_t, __Float16x8_t);
long int __builtin_aarch64_sqabsdi(long int);
short int __builtin_aarch64_sqabshi(short int);
signed char __builtin_aarch64_sqabsqi(signed char);
int __builtin_aarch64_sqabssi(int);
long int __builtin_aarch64_sqadddi(long int, long int);
short int __builtin_aarch64_sqaddhi(short int, short int);
signed char __builtin_aarch64_sqaddqi(signed char, signed char);
int __builtin_aarch64_sqaddsi(int, int);
int __builtin_aarch64_sqdmlalhi(int, short int, short int);
int __builtin_aarch64_sqdmlal_lanehi(int, short int, __Int16x4_t, int);
int __builtin_aarch64_sqdmlal_laneqhi(int, short int, __Int16x8_t, int);
long int __builtin_aarch64_sqdmlal_laneqsi(long int, int, __Int32x4_t, int);
long int __builtin_aarch64_sqdmlal_lanesi(long int, int, __Int32x2_t, int);
long int __builtin_aarch64_sqdmlalsi(long int, int, int);
int __builtin_aarch64_sqdmlslhi(int, short int, short int);
int __builtin_aarch64_sqdmlsl_lanehi(int, short int, __Int16x4_t, int);
int __builtin_aarch64_sqdmlsl_laneqhi(int, short int, __Int16x8_t, int);
long int __builtin_aarch64_sqdmlsl_laneqsi(long int, int, __Int32x4_t, int);
long int __builtin_aarch64_sqdmlsl_lanesi(long int, int, __Int32x2_t, int);
long int __builtin_aarch64_sqdmlslsi(long int, int, int);
short int __builtin_aarch64_sqdmulhhi(short int, short int);
short int __builtin_aarch64_sqdmulh_lanehi(short int, __Int16x4_t, int);
short int __builtin_aarch64_sqdmulh_laneqhi(short int, __Int16x8_t, int);
int __builtin_aarch64_sqdmulh_laneqsi(int, __Int32x4_t, int);
int __builtin_aarch64_sqdmulh_lanesi(int, __Int32x2_t, int);
int __builtin_aarch64_sqdmulhsi(int, int);
int __builtin_aarch64_sqdmullhi(short int, short int);
int __builtin_aarch64_sqdmull_lanehi(short int, __Int16x4_t, int);
int __builtin_aarch64_sqdmull_laneqhi(short int, __Int16x8_t, int);
long int __builtin_aarch64_sqdmull_laneqsi(int, __Int32x4_t, int);
long int __builtin_aarch64_sqdmull_lanesi(int, __Int32x2_t, int);
long int __builtin_aarch64_sqdmullsi(int, int);
int __builtin_aarch64_sqmovndi(long int);
signed char __builtin_aarch64_sqmovnhi(short int);
short int __builtin_aarch64_sqmovnsi(int);
int __builtin_aarch64_sqmovundi(long int);
signed char __builtin_aarch64_sqmovunhi(short int);
short int __builtin_aarch64_sqmovunsi(int);
long int __builtin_aarch64_sqnegdi(long int);
short int __builtin_aarch64_sqneghi(short int);
signed char __builtin_aarch64_sqnegqi(signed char);
int __builtin_aarch64_sqnegsi(int);
short int __builtin_aarch64_sqrdmlahhi(short int, short int, short int);
short int __builtin_aarch64_sqrdmlah_lanehi(short int, short int, __Int16x4_t, int);
short int __builtin_aarch64_sqrdmlah_laneqhi(short int, short int, __Int16x8_t, int);
int __builtin_aarch64_sqrdmlah_laneqsi(int, int, __Int32x4_t, int);
int __builtin_aarch64_sqrdmlah_lanesi(int, int, __Int32x2_t, int);
int __builtin_aarch64_sqrdmlahsi(int, int, int);
short int __builtin_aarch64_sqrdmlshhi(short int, short int, short int);
short int __builtin_aarch64_sqrdmlsh_lanehi(short int, short int, __Int16x4_t, int);
short int __builtin_aarch64_sqrdmlsh_laneqhi(short int, short int, __Int16x8_t, int);
int __builtin_aarch64_sqrdmlsh_laneqsi(int, int, __Int32x4_t, int);
int __builtin_aarch64_sqrdmlsh_lanesi(int, int, __Int32x2_t, int);
int __builtin_aarch64_sqrdmlshsi(int, int, int);
short int __builtin_aarch64_sqrdmulhhi(short int, short int);
short int __builtin_aarch64_sqrdmulh_lanehi(short int, __Int16x4_t, int);
short int __builtin_aarch64_sqrdmulh_laneqhi(short int, __Int16x8_t, int);
int __builtin_aarch64_sqrdmulh_laneqsi(int, __Int32x4_t, int);
int __builtin_aarch64_sqrdmulh_lanesi(int, __Int32x2_t, int);
int __builtin_aarch64_sqrdmulhsi(int, int);
long int __builtin_aarch64_sqrshldi(long int, long int);
short int __builtin_aarch64_sqrshlhi(short int, short int);
signed char __builtin_aarch64_sqrshlqi(signed char, signed char);
int __builtin_aarch64_sqrshlsi(int, int);
int __builtin_aarch64_sqrshrn_ndi(long int, int);
signed char __builtin_aarch64_sqrshrn_nhi(short int, int);
short int __builtin_aarch64_sqrshrn_nsi(int, int);
int __builtin_aarch64_sqrshrun_ndi(long int, int);
signed char __builtin_aarch64_sqrshrun_nhi(short int, int);
short int __builtin_aarch64_sqrshrun_nsi(int, int);
double __builtin_aarch64_sqrtdf(double);
__fp16 __builtin_aarch64_sqrthf(__fp16);
__Float16x4_t __builtin_aarch64_sqrtv4hf(__Float16x4_t);
__Float16x8_t __builtin_aarch64_sqrtv8hf(__Float16x8_t);
long int __builtin_aarch64_sqshldi(long int, long int);
short int __builtin_aarch64_sqshlhi(short int, short int);
long int __builtin_aarch64_sqshl_ndi(long int, int);
short int __builtin_aarch64_sqshl_nhi(short int, int);
signed char __builtin_aarch64_sqshl_nqi(signed char, int);
int __builtin_aarch64_sqshl_nsi(int, int);
signed char __builtin_aarch64_sqshlqi(signed char, signed char);
int __builtin_aarch64_sqshlsi(int, int);
long unsigned int __builtin_aarch64_sqshlu_ndi_uss(long int, int);
short unsigned int __builtin_aarch64_sqshlu_nhi_uss(short int, int);
unsigned char __builtin_aarch64_sqshlu_nqi_uss(signed char, int);
unsigned int __builtin_aarch64_sqshlu_nsi_uss(int, int);
int __builtin_aarch64_sqshrn_ndi(long int, int);
signed char __builtin_aarch64_sqshrn_nhi(short int, int);
short int __builtin_aarch64_sqshrn_nsi(int, int);
int __builtin_aarch64_sqshrun_ndi(long int, int);
signed char __builtin_aarch64_sqshrun_nhi(short int, int);
short int __builtin_aarch64_sqshrun_nsi(int, int);
long int __builtin_aarch64_sqsubdi(long int, long int);
short int __builtin_aarch64_sqsubhi(short int, short int);
signed char __builtin_aarch64_sqsubqi(signed char, signed char);
int __builtin_aarch64_sqsubsi(int, int);
long int __builtin_aarch64_srshldi(long int, long int);
long int __builtin_aarch64_srshr_ndi(long int, int);
long int __builtin_aarch64_srsra_ndi(long int, long int, int);
long int __builtin_aarch64_sshldi(long int, long int);
long int __builtin_aarch64_ssli_ndi(long int, long int, int);
__Poly64_t __builtin_aarch64_ssli_ndi_ppps(__Poly64_t, __Poly64_t, int);
__Poly64x2_t __builtin_aarch64_ssli_nv2di_ppps(__Poly64x2_t, __Poly64x2_t, int);
long int __builtin_aarch64_ssra_ndi(long int, long int, int);
long int __builtin_aarch64_ssri_ndi(long int, long int, int);
void __builtin_aarch64_st1v16qi(signed char*, __Int8x16_t);
void __builtin_aarch64_st1v2df(double*, __Float64x2_t);
void __builtin_aarch64_st1v2di(long int*, __Int64x2_t);
void __builtin_aarch64_st1v2di_sp(long int*, __Poly64x2_t);
void __builtin_aarch64_st1v2sf(float*, __Float32x2_t);
void __builtin_aarch64_st1v2si(int*, __Int32x2_t);
void __builtin_aarch64_st1v4hf(__fp16*, __Float16x4_t);
void __builtin_aarch64_st1v4hi(short int*, __Int16x4_t);
void __builtin_aarch64_st1v4sf(float*, __Float32x4_t);
void __builtin_aarch64_st1v4si(int*, __Int32x4_t);
void __builtin_aarch64_st1v8hf(__fp16*, __Float16x8_t);
void __builtin_aarch64_st1v8hi(short int*, __Int16x8_t);
void __builtin_aarch64_st1v8qi(signed char*, __Int8x8_t);
void __builtin_aarch64_st1x2df(double*, __builtin_aarch64_simd_oi);
void __builtin_aarch64_st1x2di(long int*, __builtin_aarch64_simd_oi);
void __builtin_aarch64_st1x2v16qi(signed char*, __builtin_aarch64_simd_oi);
void __builtin_aarch64_st1x2v2df(double*, __builtin_aarch64_simd_oi);
void __builtin_aarch64_st1x2v2di(long int*, __builtin_aarch64_simd_oi);
void __builtin_aarch64_st1x2v2sf(float*, __builtin_aarch64_simd_oi);
void __builtin_aarch64_st1x2v2si(int*, __builtin_aarch64_simd_oi);
void __builtin_aarch64_st1x2v4hf(__fp16*, __builtin_aarch64_simd_oi);
void __builtin_aarch64_st1x2v4hi(short int*, __builtin_aarch64_simd_oi);
void __builtin_aarch64_st1x2v4sf(float*, __builtin_aarch64_simd_oi);
void __builtin_aarch64_st1x2v4si(int*, __builtin_aarch64_simd_oi);
void __builtin_aarch64_st1x2v8hf(__fp16*, __builtin_aarch64_simd_oi);
void __builtin_aarch64_st1x2v8hi(short int*, __builtin_aarch64_simd_oi);
void __builtin_aarch64_st1x2v8qi(signed char*, __builtin_aarch64_simd_oi);
void __builtin_aarch64_st1x3df(double*, __builtin_aarch64_simd_ci);
void __builtin_aarch64_st1x3di(long int*, __builtin_aarch64_simd_ci);
void __builtin_aarch64_st1x3v16qi(signed char*, __builtin_aarch64_simd_ci);
void __builtin_aarch64_st1x3v2df(double*, __builtin_aarch64_simd_ci);
void __builtin_aarch64_st1x3v2di(long int*, __builtin_aarch64_simd_ci);
void __builtin_aarch64_st1x3v2sf(float*, __builtin_aarch64_simd_ci);
void __builtin_aarch64_st1x3v2si(int*, __builtin_aarch64_simd_ci);
void __builtin_aarch64_st1x3v4hf(__fp16*, __builtin_aarch64_simd_ci);
void __builtin_aarch64_st1x3v4hi(short int*, __builtin_aarch64_simd_ci);
void __builtin_aarch64_st1x3v4sf(float*, __builtin_aarch64_simd_ci);
void __builtin_aarch64_st1x3v4si(int*, __builtin_aarch64_simd_ci);
void __builtin_aarch64_st1x3v8hf(__fp16*, __builtin_aarch64_simd_ci);
void __builtin_aarch64_st1x3v8hi(short int*, __builtin_aarch64_simd_ci);
void __builtin_aarch64_st1x3v8qi(signed char*, __builtin_aarch64_simd_ci);
void __builtin_aarch64_st2df(double*, __builtin_aarch64_simd_oi);
void __builtin_aarch64_st2di(long int*, __builtin_aarch64_simd_oi);
void __builtin_aarch64_st2v16qi(signed char*, __builtin_aarch64_simd_oi);
void __builtin_aarch64_st2v2df(double*, __builtin_aarch64_simd_oi);
void __builtin_aarch64_st2v2di(long int*, __builtin_aarch64_simd_oi);
void __builtin_aarch64_st2v2sf(float*, __builtin_aarch64_simd_oi);
void __builtin_aarch64_st2v2si(int*, __builtin_aarch64_simd_oi);
void __builtin_aarch64_st2v4hf(__fp16*, __builtin_aarch64_simd_oi);
void __builtin_aarch64_st2v4hi(short int*, __builtin_aarch64_simd_oi);
void __builtin_aarch64_st2v4sf(float*, __builtin_aarch64_simd_oi);
void __builtin_aarch64_st2v4si(int*, __builtin_aarch64_simd_oi);
void __builtin_aarch64_st2v8hf(__fp16*, __builtin_aarch64_simd_oi);
void __builtin_aarch64_st2v8hi(short int*, __builtin_aarch64_simd_oi);
void __builtin_aarch64_st2v8qi(signed char*, __builtin_aarch64_simd_oi);
void __builtin_aarch64_st3df(double*, __builtin_aarch64_simd_ci);
void __builtin_aarch64_st3di(long int*, __builtin_aarch64_simd_ci);
void __builtin_aarch64_st3v16qi(signed char*, __builtin_aarch64_simd_ci);
void __builtin_aarch64_st3v2df(double*, __builtin_aarch64_simd_ci);
void __builtin_aarch64_st3v2di(long int*, __builtin_aarch64_simd_ci);
void __builtin_aarch64_st3v2sf(float*, __builtin_aarch64_simd_ci);
void __builtin_aarch64_st3v2si(int*, __builtin_aarch64_simd_ci);
void __builtin_aarch64_st3v4hf(__fp16*, __builtin_aarch64_simd_ci);
void __builtin_aarch64_st3v4hi(short int*, __builtin_aarch64_simd_ci);
void __builtin_aarch64_st3v4sf(float*, __builtin_aarch64_simd_ci);
void __builtin_aarch64_st3v4si(int*, __builtin_aarch64_simd_ci);
void __builtin_aarch64_st3v8hf(__fp16*, __builtin_aarch64_simd_ci);
void __builtin_aarch64_st3v8hi(short int*, __builtin_aarch64_simd_ci);
void __builtin_aarch64_st3v8qi(signed char*, __builtin_aarch64_simd_ci);
void __builtin_aarch64_st4df(double*, __builtin_aarch64_simd_xi);
void __builtin_aarch64_st4di(long int*, __builtin_aarch64_simd_xi);
void __builtin_aarch64_st4v16qi(signed char*, __builtin_aarch64_simd_xi);
void __builtin_aarch64_st4v2df(double*, __builtin_aarch64_simd_xi);
void __builtin_aarch64_st4v2di(long int*, __builtin_aarch64_simd_xi);
void __builtin_aarch64_st4v2sf(float*, __builtin_aarch64_simd_xi);
void __builtin_aarch64_st4v2si(int*, __builtin_aarch64_simd_xi);
void __builtin_aarch64_st4v4hf(__fp16*, __builtin_aarch64_simd_xi);
void __builtin_aarch64_st4v4hi(short int*, __builtin_aarch64_simd_xi);
void __builtin_aarch64_st4v4sf(float*, __builtin_aarch64_simd_xi);
void __builtin_aarch64_st4v4si(int*, __builtin_aarch64_simd_xi);
void __builtin_aarch64_st4v8hf(__fp16*, __builtin_aarch64_simd_xi);
void __builtin_aarch64_st4v8hi(short int*, __builtin_aarch64_simd_xi);
void __builtin_aarch64_st4v8qi(signed char*, __builtin_aarch64_simd_xi);
long int __builtin_aarch64_suqadddi_ssu(long int, long unsigned int);
short int __builtin_aarch64_suqaddhi_ssu(short int, short unsigned int);
signed char __builtin_aarch64_suqaddqi_ssu(signed char, unsigned char);
int __builtin_aarch64_suqaddsi_ssu(int, unsigned int);
__Int8x16_t __builtin_aarch64_tbl3v16qi(__builtin_aarch64_simd_oi, __Int8x16_t);
__Int8x16_t __builtin_aarch64_tbx4v16qi(__Int8x16_t, __builtin_aarch64_simd_oi, __Int8x16_t);
__fp16 __builtin_aarch64_ucvtfdihf_sus(long unsigned int, int);
double __builtin_aarch64_ucvtfdi_sus(long unsigned int, int);
__fp16 __builtin_aarch64_ucvtfhi_sus(short unsigned int, int);
__fp16 __builtin_aarch64_ucvtfsihf_sus(unsigned int, int);
float __builtin_aarch64_ucvtfsi_sus(unsigned int, int);
__Float64x2_t __builtin_aarch64_ucvtfv2di_sus(__Uint64x2_t, int);
__Float32x2_t __builtin_aarch64_ucvtfv2si_sus(__Uint32x2_t, int);
__Float16x4_t __builtin_aarch64_ucvtfv4hi_sus(__Uint16x4_t, int);
__Float32x4_t __builtin_aarch64_ucvtfv4si_sus(__Uint32x4_t, int);
__Float16x8_t __builtin_aarch64_ucvtfv8hi_sus(__Uint16x8_t, int);
__Uint32x4_t __builtin_aarch64_udot_laneqv16qi_uuuus(__Uint32x4_t, __Uint8x16_t, __Uint8x16_t, int);
__Uint32x2_t __builtin_aarch64_udot_laneqv8qi_uuuus(__Uint32x2_t, __Uint8x8_t, __Uint8x16_t, int);
__Uint32x4_t __builtin_aarch64_udot_lanev16qi_uuuus(__Uint32x4_t, __Uint8x16_t, __Uint8x8_t, int);
__Uint32x2_t __builtin_aarch64_udot_lanev8qi_uuuus(__Uint32x2_t, __Uint8x8_t, __Uint8x8_t, int);
__Uint32x4_t __builtin_aarch64_udotv16qi_uuuu(__Uint32x4_t, __Uint8x16_t, __Uint8x16_t);
__Uint32x2_t __builtin_aarch64_udotv8qi_uuuu(__Uint32x2_t, __Uint8x8_t, __Uint8x8_t);
long unsigned int __builtin_aarch64_uqadddi_uuu(long unsigned int, long unsigned int);
short unsigned int __builtin_aarch64_uqaddhi_uuu(short unsigned int, short unsigned int);
unsigned char __builtin_aarch64_uqaddqi_uuu(unsigned char, unsigned char);
unsigned int __builtin_aarch64_uqaddsi_uuu(unsigned int, unsigned int);
int __builtin_aarch64_uqmovndi(long int);
signed char __builtin_aarch64_uqmovnhi(short int);
short int __builtin_aarch64_uqmovnsi(int);
long unsigned int __builtin_aarch64_uqrshldi_uus(long unsigned int, long int);
short unsigned int __builtin_aarch64_uqrshlhi_uus(short unsigned int, short int);
unsigned char __builtin_aarch64_uqrshlqi_uus(unsigned char, signed char);
unsigned int __builtin_aarch64_uqrshlsi_uus(unsigned int, int);
unsigned int __builtin_aarch64_uqrshrn_ndi_uus(long unsigned int, int);
unsigned char __builtin_aarch64_uqrshrn_nhi_uus(short unsigned int, int);
short unsigned int __builtin_aarch64_uqrshrn_nsi_uus(unsigned int, int);
long unsigned int __builtin_aarch64_uqshldi_uus(long unsigned int, long int);
short unsigned int __builtin_aarch64_uqshlhi_uus(short unsigned int, short int);
long unsigned int __builtin_aarch64_uqshl_ndi_uus(long unsigned int, int);
short unsigned int __builtin_aarch64_uqshl_nhi_uus(short unsigned int, int);
unsigned char __builtin_aarch64_uqshl_nqi_uus(unsigned char, int);
unsigned int __builtin_aarch64_uqshl_nsi_uus(unsigned int, int);
unsigned char __builtin_aarch64_uqshlqi_uus(unsigned char, signed char);
unsigned int __builtin_aarch64_uqshlsi_uus(unsigned int, int);
unsigned int __builtin_aarch64_uqshrn_ndi_uus(long unsigned int, int);
unsigned char __builtin_aarch64_uqshrn_nhi_uus(short unsigned int, int);
short unsigned int __builtin_aarch64_uqshrn_nsi_uus(unsigned int, int);
long unsigned int __builtin_aarch64_uqsubdi_uuu(long unsigned int, long unsigned int);
short unsigned int __builtin_aarch64_uqsubhi_uuu(short unsigned int, short unsigned int);
unsigned char __builtin_aarch64_uqsubqi_uuu(unsigned char, unsigned char);
unsigned int __builtin_aarch64_uqsubsi_uuu(unsigned int, unsigned int);
long unsigned int __builtin_aarch64_urshldi_uus(long unsigned int, long int);
long unsigned int __builtin_aarch64_urshr_ndi_uus(long unsigned int, int);
long unsigned int __builtin_aarch64_ursra_ndi_uuus(long unsigned int, long unsigned int, int);
long unsigned int __builtin_aarch64_ushldi_uus(long unsigned int, long int);
long unsigned int __builtin_aarch64_usli_ndi_uuus(long unsigned int, long unsigned int, int);
long unsigned int __builtin_aarch64_usqadddi_uus(long unsigned int, long int);
short unsigned int __builtin_aarch64_usqaddhi_uus(short unsigned int, short int);
unsigned char __builtin_aarch64_usqaddqi_uus(unsigned char, signed char);
unsigned int __builtin_aarch64_usqaddsi_uus(unsigned int, int);
long unsigned int __builtin_aarch64_usra_ndi_uuus(long unsigned int, long unsigned int, int);
long unsigned int __builtin_aarch64_usri_ndi_uuus(long unsigned int, long unsigned int, int);
__Uint64x2_t __builtin_aarch64_xarqv2di_uuus(__Uint64x2_t, __Uint64x2_t, int);

void* __builtin_aarch64_autia1716(void*, long unsigned int);
void* __builtin_aarch64_autib1716(void*, long unsigned int);
__bf16 __builtin_aarch64_bfcvtbf(float);
__Bfloat16x8_t __builtin_aarch64_bfcvtn2v8bf(__Bfloat16x8_t, __Float32x4_t);
__Bfloat16x8_t __builtin_aarch64_bfcvtn_qv8bf(__Float32x4_t);
__Bfloat16x4_t __builtin_aarch64_bfcvtnv4bf(__Float32x4_t);
__Float32x2_t __builtin_aarch64_bfdot_laneqv2sf(__Float32x2_t, __Bfloat16x4_t, __Bfloat16x8_t, int);
__Float32x4_t __builtin_aarch64_bfdot_laneqv4sf(__Float32x4_t, __Bfloat16x8_t, __Bfloat16x8_t, int);
__Float32x2_t __builtin_aarch64_bfdot_lanev2sf(__Float32x2_t, __Bfloat16x4_t, __Bfloat16x4_t, int);
__Float32x4_t __builtin_aarch64_bfdot_lanev4sf(__Float32x4_t, __Bfloat16x8_t, __Bfloat16x4_t, int);
__Float32x2_t __builtin_aarch64_bfdotv2sf(__Float32x2_t, __Bfloat16x4_t, __Bfloat16x4_t);
__Float32x4_t __builtin_aarch64_bfdotv4sf(__Float32x4_t, __Bfloat16x8_t, __Bfloat16x8_t);
__Float32x4_t __builtin_aarch64_bfmlalb_lane_qv4sf(__Float32x4_t, __Bfloat16x8_t, __Bfloat16x8_t, int);
__Float32x4_t __builtin_aarch64_bfmlalb_lanev4sf(__Float32x4_t, __Bfloat16x8_t, __Bfloat16x4_t, int);
__Float32x4_t __builtin_aarch64_bfmlalbv4sf(__Float32x4_t, __Bfloat16x8_t, __Bfloat16x8_t);
__Float32x4_t __builtin_aarch64_bfmlalt_lane_qv4sf(__Float32x4_t, __Bfloat16x8_t, __Bfloat16x8_t, int);
__Float32x4_t __builtin_aarch64_bfmlalt_lanev4sf(__Float32x4_t, __Bfloat16x8_t, __Bfloat16x4_t, int);
__Float32x4_t __builtin_aarch64_bfmlaltv4sf(__Float32x4_t, __Bfloat16x8_t, __Bfloat16x8_t);
__Float32x4_t __builtin_aarch64_bfmmlaqv4sf(__Float32x4_t, __Bfloat16x8_t, __Bfloat16x8_t);
__Uint64x2_t __builtin_aarch64_bswapv2di_uu(__Uint64x2_t);
__Uint32x2_t __builtin_aarch64_bswapv2si_uu(__Uint32x2_t);
__Uint16x4_t __builtin_aarch64_bswapv4hi_uu(__Uint16x4_t);
__Uint32x4_t __builtin_aarch64_bswapv4si_uu(__Uint32x4_t);
__Uint16x8_t __builtin_aarch64_bswapv8hi_uu(__Uint16x8_t);
long unsigned int __builtin_aarch64_cmeqdf_uss(double, double);
unsigned int __builtin_aarch64_cmeqsf_uss(float, float);
__Uint64x2_t __builtin_aarch64_cmeqv2df_uss(__Float64x2_t, __Float64x2_t);
__Uint32x2_t __builtin_aarch64_cmeqv2sf_uss(__Float32x2_t, __Float32x2_t);
__Uint32x4_t __builtin_aarch64_cmeqv4sf_uss(__Float32x4_t, __Float32x4_t);
long unsigned int __builtin_aarch64_cmgedf_uss(double, double);
unsigned int __builtin_aarch64_cmgesf_uss(float, float);
__Uint64x2_t __builtin_aarch64_cmgev2df_uss(__Float64x2_t, __Float64x2_t);
__Uint32x2_t __builtin_aarch64_cmgev2sf_uss(__Float32x2_t, __Float32x2_t);
__Uint32x4_t __builtin_aarch64_cmgev4sf_uss(__Float32x4_t, __Float32x4_t);
long unsigned int __builtin_aarch64_cmgtdf_uss(double, double);
unsigned int __builtin_aarch64_cmgtsf_uss(float, float);
__Uint64x2_t __builtin_aarch64_cmgtv2df_uss(__Float64x2_t, __Float64x2_t);
__Uint32x2_t __builtin_aarch64_cmgtv2sf_uss(__Float32x2_t, __Float32x2_t);
__Uint32x4_t __builtin_aarch64_cmgtv4sf_uss(__Float32x4_t, __Float32x4_t);
long unsigned int __builtin_aarch64_cmledf_uss(double, double);
unsigned int __builtin_aarch64_cmlesf_uss(float, float);
__Uint64x2_t __builtin_aarch64_cmlev2df_uss(__Float64x2_t, __Float64x2_t);
__Uint32x2_t __builtin_aarch64_cmlev2sf_uss(__Float32x2_t, __Float32x2_t);
__Uint32x4_t __builtin_aarch64_cmlev4sf_uss(__Float32x4_t, __Float32x4_t);
long unsigned int __builtin_aarch64_cmltdf_uss(double, double);
unsigned int __builtin_aarch64_cmltsf_uss(float, float);
__Uint64x2_t __builtin_aarch64_cmltv2df_uss(__Float64x2_t, __Float64x2_t);
__Uint32x2_t __builtin_aarch64_cmltv2sf_uss(__Float32x2_t, __Float32x2_t);
__Uint32x4_t __builtin_aarch64_cmltv4sf_uss(__Float32x4_t, __Float32x4_t);
__Bfloat16x8_t __builtin_aarch64_combinev4bf(__Bfloat16x4_t, __Bfloat16x4_t);
__Int32x2_t __builtin_aarch64_ctzv2si(__Int32x2_t);
__Int32x4_t __builtin_aarch64_ctzv4si(__Int32x4_t);
long unsigned int __builtin_aarch64_facgedf_uss(double, double);
unsigned int __builtin_aarch64_facgesf_uss(float, float);
__Uint64x2_t __builtin_aarch64_facgev2df_uss(__Float64x2_t, __Float64x2_t);
__Uint32x2_t __builtin_aarch64_facgev2sf_uss(__Float32x2_t, __Float32x2_t);
__Uint32x4_t __builtin_aarch64_facgev4sf_uss(__Float32x4_t, __Float32x4_t);
long unsigned int __builtin_aarch64_facgtdf_uss(double, double);
unsigned int __builtin_aarch64_facgtsf_uss(float, float);
__Uint64x2_t __builtin_aarch64_facgtv2df_uss(__Float64x2_t, __Float64x2_t);
__Uint32x2_t __builtin_aarch64_facgtv2sf_uss(__Float32x2_t, __Float32x2_t);
__Uint32x4_t __builtin_aarch64_facgtv4sf_uss(__Float32x4_t, __Float32x4_t);
long unsigned int __builtin_aarch64_facledf_uss(double, double);
unsigned int __builtin_aarch64_faclesf_uss(float, float);
__Uint64x2_t __builtin_aarch64_faclev2df_uss(__Float64x2_t, __Float64x2_t);
__Uint32x2_t __builtin_aarch64_faclev2sf_uss(__Float32x2_t, __Float32x2_t);
__Uint32x4_t __builtin_aarch64_faclev4sf_uss(__Float32x4_t, __Float32x4_t);
long unsigned int __builtin_aarch64_facltdf_uss(double, double);
unsigned int __builtin_aarch64_facltsf_uss(float, float);
__Uint64x2_t __builtin_aarch64_facltv2df_uss(__Float64x2_t, __Float64x2_t);
__Uint32x2_t __builtin_aarch64_facltv2sf_uss(__Float32x2_t, __Float32x2_t);
__Uint32x4_t __builtin_aarch64_facltv4sf_uss(__Float32x4_t, __Float32x4_t);
__Float64x2_t __builtin_aarch64_fcmla_lane0v2df(__Float64x2_t, __Float64x2_t, __Float64x2_t, int);
__Float64x2_t __builtin_aarch64_fcmla_lane180v2df(__Float64x2_t, __Float64x2_t, __Float64x2_t, int);
__Float64x2_t __builtin_aarch64_fcmla_lane270v2df(__Float64x2_t, __Float64x2_t, __Float64x2_t, int);
__Float64x2_t __builtin_aarch64_fcmla_lane90v2df(__Float64x2_t, __Float64x2_t, __Float64x2_t, int);
long int __builtin_aarch64_fix_truncdfdi(double);
int __builtin_aarch64_fix_truncdfsi(double);
long int __builtin_aarch64_fix_truncsfdi(float);
int __builtin_aarch64_fix_truncsfsi(float);
long unsigned int __builtin_aarch64_fixuns_truncdfdi_us(double);
unsigned int __builtin_aarch64_fixuns_truncdfsi_us(double);
long unsigned int __builtin_aarch64_fixuns_truncsfdi_us(float);
unsigned int __builtin_aarch64_fixuns_truncsfsi_us(float);
float __builtin_aarch64_fmaxsf(float, float);
float __builtin_aarch64_fminsf(float, float);
__Float64x2_t __builtin_aarch64_fnmav2df(__Float64x2_t, __Float64x2_t, __Float64x2_t);
__Float32x2_t __builtin_aarch64_fnmav2sf(__Float32x2_t, __Float32x2_t, __Float32x2_t);
__Float32x4_t __builtin_aarch64_fnmav4sf(__Float32x4_t, __Float32x4_t, __Float32x4_t);
double __builtin_aarch64_frint32xdf(double);
float __builtin_aarch64_frint32xsf(float);
__Float64x2_t __builtin_aarch64_frint32xv2df(__Float64x2_t);
__Float32x2_t __builtin_aarch64_frint32xv2sf(__Float32x2_t);
__Float32x4_t __builtin_aarch64_frint32xv4sf(__Float32x4_t);
double __builtin_aarch64_frint32zdf(double);
float __builtin_aarch64_frint32zsf(float);
__Float64x2_t __builtin_aarch64_frint32zv2df(__Float64x2_t);
__Float32x2_t __builtin_aarch64_frint32zv2sf(__Float32x2_t);
__Float32x4_t __builtin_aarch64_frint32zv4sf(__Float32x4_t);
double __builtin_aarch64_frint64xdf(double);
float __builtin_aarch64_frint64xsf(float);
__Float64x2_t __builtin_aarch64_frint64xv2df(__Float64x2_t);
__Float32x2_t __builtin_aarch64_frint64xv2sf(__Float32x2_t);
__Float32x4_t __builtin_aarch64_frint64xv4sf(__Float32x4_t);
double __builtin_aarch64_frint64zdf(double);
float __builtin_aarch64_frint64zsf(float);
__Float64x2_t __builtin_aarch64_frint64zv2df(__Float64x2_t);
__Float32x2_t __builtin_aarch64_frint64zv2sf(__Float32x2_t);
__Float32x4_t __builtin_aarch64_frint64zv4sf(__Float32x4_t);
__Bfloat16x4_t __builtin_aarch64_get_dregciv4bf(__builtin_aarch64_simd_ci, int);
__Bfloat16x4_t __builtin_aarch64_get_dregoiv4bf(__builtin_aarch64_simd_oi, int);
__Bfloat16x4_t __builtin_aarch64_get_dregxiv4bf(__builtin_aarch64_simd_xi, int);
__Bfloat16x8_t __builtin_aarch64_get_qregciv8bf(__builtin_aarch64_simd_ci, int);
__Bfloat16x8_t __builtin_aarch64_get_qregoiv8bf(__builtin_aarch64_simd_oi, int);
__Bfloat16x8_t __builtin_aarch64_get_qregxiv8bf(__builtin_aarch64_simd_xi, int);
__Int64x2_t __builtin_aarch64_intrinsic_vec_smult_lo_v2si(__Int32x2_t, __Int32x2_t);
__Int32x4_t __builtin_aarch64_intrinsic_vec_smult_lo_v4hi(__Int16x4_t, __Int16x4_t);
__Int16x8_t __builtin_aarch64_intrinsic_vec_smult_lo_v8qi(__Int8x8_t, __Int8x8_t);
__Uint64x2_t __builtin_aarch64_intrinsic_vec_umult_lo_v2si_uuu(__Uint32x2_t, __Uint32x2_t);
__Uint32x4_t __builtin_aarch64_intrinsic_vec_umult_lo_v4hi_uuu(__Uint16x4_t, __Uint16x4_t);
__Uint16x8_t __builtin_aarch64_intrinsic_vec_umult_lo_v8qi_uuu(__Uint8x8_t, __Uint8x8_t);
int __builtin_aarch64_jcvtzs(double);
void __builtin_aarch64_ld1v2di_sp(long int*, __Poly64_t);
__Bfloat16x4_t __builtin_aarch64_ld1v4bf(const __bf16*);
__Bfloat16x8_t __builtin_aarch64_ld1v8bf(const __bf16*);
__builtin_aarch64_simd_oi __builtin_aarch64_ld1x2v4bf(const __bf16*);
__builtin_aarch64_simd_oi __builtin_aarch64_ld1x2v8bf(const __bf16*);
__builtin_aarch64_simd_ci __builtin_aarch64_ld1x3v4bf(const __bf16*);
__builtin_aarch64_simd_ci __builtin_aarch64_ld1x3v8bf(const __bf16*);
__builtin_aarch64_simd_xi __builtin_aarch64_ld1x4df(const double*);
__builtin_aarch64_simd_xi __builtin_aarch64_ld1x4di(const long int*);
__builtin_aarch64_simd_xi __builtin_aarch64_ld1x4v16qi(const signed char*);
__builtin_aarch64_simd_xi __builtin_aarch64_ld1x4v2df(const double*);
__builtin_aarch64_simd_xi __builtin_aarch64_ld1x4v2di(const long int*);
__builtin_aarch64_simd_xi __builtin_aarch64_ld1x4v2sf(const float*);
__builtin_aarch64_simd_xi __builtin_aarch64_ld1x4v2si(const int*);
__builtin_aarch64_simd_xi __builtin_aarch64_ld1x4v4bf(const __bf16*);
__builtin_aarch64_simd_xi __builtin_aarch64_ld1x4v4hf(const __fp16*);
__builtin_aarch64_simd_xi __builtin_aarch64_ld1x4v4hi(const short int*);
__builtin_aarch64_simd_xi __builtin_aarch64_ld1x4v4sf(const float*);
__builtin_aarch64_simd_xi __builtin_aarch64_ld1x4v4si(const int*);
__builtin_aarch64_simd_xi __builtin_aarch64_ld1x4v8bf(const __bf16*);
__builtin_aarch64_simd_xi __builtin_aarch64_ld1x4v8hf(const __fp16*);
__builtin_aarch64_simd_xi __builtin_aarch64_ld1x4v8hi(const short int*);
__builtin_aarch64_simd_xi __builtin_aarch64_ld1x4v8qi(const signed char*);
__builtin_aarch64_simd_oi __builtin_aarch64_ld2_lanev4bf(const __bf16*, __builtin_aarch64_simd_oi, int);
__builtin_aarch64_simd_oi __builtin_aarch64_ld2_lanev8bf(const __bf16*, __builtin_aarch64_simd_oi, int);
__builtin_aarch64_simd_oi __builtin_aarch64_ld2rv4bf(const __bf16*);
__builtin_aarch64_simd_oi __builtin_aarch64_ld2rv8bf(const __bf16*);
__builtin_aarch64_simd_oi __builtin_aarch64_ld2v4bf(const __bf16*);
__builtin_aarch64_simd_oi __builtin_aarch64_ld2v8bf(const __bf16*);
__builtin_aarch64_simd_ci __builtin_aarch64_ld3_lanev4bf(const __bf16*, __builtin_aarch64_simd_ci, int);
__builtin_aarch64_simd_ci __builtin_aarch64_ld3_lanev8bf(const __bf16*, __builtin_aarch64_simd_ci, int);
__builtin_aarch64_simd_ci __builtin_aarch64_ld3rv4bf(const __bf16*);
__builtin_aarch64_simd_ci __builtin_aarch64_ld3rv8bf(const __bf16*);
__builtin_aarch64_simd_ci __builtin_aarch64_ld3v4bf(const __bf16*);
__builtin_aarch64_simd_ci __builtin_aarch64_ld3v8bf(const __bf16*);
__builtin_aarch64_simd_xi __builtin_aarch64_ld4_lanev4bf(const __bf16*, __builtin_aarch64_simd_xi, int);
__builtin_aarch64_simd_xi __builtin_aarch64_ld4_lanev8bf(const __bf16*, __builtin_aarch64_simd_xi, int);
__builtin_aarch64_simd_xi __builtin_aarch64_ld4rv4bf(const __bf16*);
__builtin_aarch64_simd_xi __builtin_aarch64_ld4rv8bf(const __bf16*);
__builtin_aarch64_simd_xi __builtin_aarch64_ld4v4bf(const __bf16*);
__builtin_aarch64_simd_xi __builtin_aarch64_ld4v8bf(const __bf16*);
double __builtin_aarch64_negdf(double);
float __builtin_aarch64_negsf(float);
__Float64x2_t __builtin_aarch64_negv2df(__Float64x2_t);
__Float32x2_t __builtin_aarch64_negv2sf(__Float32x2_t);
__Float16x4_t __builtin_aarch64_negv4hf(__Float16x4_t);
__Float32x4_t __builtin_aarch64_negv4sf(__Float32x4_t);
__Float16x8_t __builtin_aarch64_negv8hf(__Float16x8_t);
void* __builtin_aarch64_pacia1716(void*, long unsigned int);
void* __builtin_aarch64_pacib1716(void*, long unsigned int);
int __builtin_aarch64_rndr(long unsigned int*);
int __builtin_aarch64_rndrrs(long unsigned int*);
double __builtin_aarch64_rsqrt_df(double);
float __builtin_aarch64_rsqrt_sf(float);
__Float64x2_t __builtin_aarch64_rsqrt_v2df(__Float64x2_t);
__Float32x2_t __builtin_aarch64_rsqrt_v2sf(__Float32x2_t);
__Float32x4_t __builtin_aarch64_rsqrt_v4sf(__Float32x4_t);
__builtin_aarch64_simd_ci __builtin_aarch64_set_qregciv8bf(__builtin_aarch64_simd_ci, __Bfloat16x8_t, int);
__builtin_aarch64_simd_oi __builtin_aarch64_set_qregoiv8bf(__builtin_aarch64_simd_oi, __Bfloat16x8_t, int);
__builtin_aarch64_simd_xi __builtin_aarch64_set_qregxiv8bf(__builtin_aarch64_simd_xi, __Bfloat16x8_t, int);
__Bfloat16x4_t __builtin_aarch64_simd_bslv4bf_suss(__Uint16x4_t, __Bfloat16x4_t, __Bfloat16x4_t);
__Bfloat16x8_t __builtin_aarch64_simd_bslv8bf_suss(__Uint16x8_t, __Bfloat16x8_t, __Bfloat16x8_t);
__Int32x4_t __builtin_aarch64_simd_smmlav16qi(__Int32x4_t, __Int8x16_t, __Int8x16_t);
__Uint32x4_t __builtin_aarch64_simd_ummlav16qi_uuuu(__Uint32x4_t, __Uint8x16_t, __Uint8x16_t);
__Int32x4_t __builtin_aarch64_simd_usmmlav16qi_ssus(__Int32x4_t, __Uint8x16_t, __Int8x16_t);
void __builtin_aarch64_st1v4bf(__bf16*, __Bfloat16x4_t);
void __builtin_aarch64_st1v8bf(__bf16*, __Bfloat16x8_t);
void __builtin_aarch64_st1x2v4bf(__bf16*, __builtin_aarch64_simd_oi);
void __builtin_aarch64_st1x2v8bf(__bf16*, __builtin_aarch64_simd_oi);
void __builtin_aarch64_st1x3v4bf(__bf16*, __builtin_aarch64_simd_ci);
void __builtin_aarch64_st1x3v8bf(__bf16*, __builtin_aarch64_simd_ci);
void __builtin_aarch64_st1x4df(double*, __builtin_aarch64_simd_xi);
void __builtin_aarch64_st1x4di(long int*, __builtin_aarch64_simd_xi);
void __builtin_aarch64_st1x4v16qi(signed char*, __builtin_aarch64_simd_xi);
void __builtin_aarch64_st1x4v2df(double*, __builtin_aarch64_simd_xi);
void __builtin_aarch64_st1x4v2di(long int*, __builtin_aarch64_simd_xi);
void __builtin_aarch64_st1x4v2sf(float*, __builtin_aarch64_simd_xi);
void __builtin_aarch64_st1x4v2si(int*, __builtin_aarch64_simd_xi);
void __builtin_aarch64_st1x4v4bf(__bf16*, __builtin_aarch64_simd_xi);
void __builtin_aarch64_st1x4v4hf(__fp16*, __builtin_aarch64_simd_xi);
void __builtin_aarch64_st1x4v4hi(short int*, __builtin_aarch64_simd_xi);
void __builtin_aarch64_st1x4v4sf(float*, __builtin_aarch64_simd_xi);
void __builtin_aarch64_st1x4v4si(int*, __builtin_aarch64_simd_xi);
void __builtin_aarch64_st1x4v8bf(__bf16*, __builtin_aarch64_simd_xi);
void __builtin_aarch64_st1x4v8hf(__fp16*, __builtin_aarch64_simd_xi);
void __builtin_aarch64_st1x4v8hi(short int*, __builtin_aarch64_simd_xi);
void __builtin_aarch64_st1x4v8qi(signed char*, __builtin_aarch64_simd_xi);
void __builtin_aarch64_st2_lanev4bf(__bf16*, __builtin_aarch64_simd_oi, int);
void __builtin_aarch64_st2_lanev8bf(__bf16*, __builtin_aarch64_simd_oi, int);
void __builtin_aarch64_st2v4bf(__bf16*, __builtin_aarch64_simd_oi);
void __builtin_aarch64_st2v8bf(__bf16*, __builtin_aarch64_simd_oi);
void __builtin_aarch64_st3_lanev4bf(__bf16*, __builtin_aarch64_simd_ci, int);
void __builtin_aarch64_st3_lanev8bf(__bf16*, __builtin_aarch64_simd_ci, int);
void __builtin_aarch64_st3v4bf(__bf16*, __builtin_aarch64_simd_ci);
void __builtin_aarch64_st3v8bf(__bf16*, __builtin_aarch64_simd_ci);
void __builtin_aarch64_st4_lanev4bf(__bf16*, __builtin_aarch64_simd_xi, int);
void __builtin_aarch64_st4_lanev8bf(__bf16*, __builtin_aarch64_simd_xi, int);
void __builtin_aarch64_st4v4bf(__bf16*, __builtin_aarch64_simd_xi);
void __builtin_aarch64_st4v8bf(__bf16*, __builtin_aarch64_simd_xi);
__Int32x4_t __builtin_aarch64_sudot_laneqv16qi_sssus(__Int32x4_t, __Int8x16_t, __Uint8x16_t, int);
__Int32x2_t __builtin_aarch64_sudot_laneqv8qi_sssus(__Int32x2_t, __Int8x8_t, __Uint8x16_t, int);
__Int32x4_t __builtin_aarch64_sudot_lanev16qi_sssus(__Int32x4_t, __Int8x16_t, __Uint8x8_t, int);
__Int32x2_t __builtin_aarch64_sudot_lanev8qi_sssus(__Int32x2_t, __Int8x8_t, __Uint8x8_t, int);
__Int8x16_t __builtin_aarch64_trn1v16qi(__Int8x16_t, __Int8x16_t);
__Float64x2_t __builtin_aarch64_trn1v2df(__Float64x2_t, __Float64x2_t);
__Int64x2_t __builtin_aarch64_trn1v2di(__Int64x2_t, __Int64x2_t);
__Float32x2_t __builtin_aarch64_trn1v2sf(__Float32x2_t, __Float32x2_t);
__Int32x2_t __builtin_aarch64_trn1v2si(__Int32x2_t, __Int32x2_t);
__Int16x4_t __builtin_aarch64_trn1v4hi(__Int16x4_t, __Int16x4_t);
__Float32x4_t __builtin_aarch64_trn1v4sf(__Float32x4_t, __Float32x4_t);
__Int32x4_t __builtin_aarch64_trn1v4si(__Int32x4_t, __Int32x4_t);
__Int16x8_t __builtin_aarch64_trn1v8hi(__Int16x8_t, __Int16x8_t);
__Int8x8_t __builtin_aarch64_trn1v8qi(__Int8x8_t, __Int8x8_t);
__Int8x16_t __builtin_aarch64_trn2v16qi(__Int8x16_t, __Int8x16_t);
__Float64x2_t __builtin_aarch64_trn2v2df(__Float64x2_t, __Float64x2_t);
__Int64x2_t __builtin_aarch64_trn2v2di(__Int64x2_t, __Int64x2_t);
__Float32x2_t __builtin_aarch64_trn2v2sf(__Float32x2_t, __Float32x2_t);
__Int32x2_t __builtin_aarch64_trn2v2si(__Int32x2_t, __Int32x2_t);
__Int16x4_t __builtin_aarch64_trn2v4hi(__Int16x4_t, __Int16x4_t);
__Float32x4_t __builtin_aarch64_trn2v4sf(__Float32x4_t, __Float32x4_t);
__Int32x4_t __builtin_aarch64_trn2v4si(__Int32x4_t, __Int32x4_t);
__Int16x8_t __builtin_aarch64_trn2v8hi(__Int16x8_t, __Int16x8_t);
__Int8x8_t __builtin_aarch64_trn2v8qi(__Int8x8_t, __Int8x8_t);
__Int32x4_t __builtin_aarch64_usdot_laneqv16qi_ssuss(__Int32x4_t, __Uint8x16_t, __Int8x16_t, int);
__Int32x2_t __builtin_aarch64_usdot_laneqv8qi_ssuss(__Int32x2_t, __Uint8x8_t, __Int8x16_t, int);
__Int32x4_t __builtin_aarch64_usdot_lanev16qi_ssuss(__Int32x4_t, __Uint8x16_t, __Int8x8_t, int);
__Int32x2_t __builtin_aarch64_usdot_lanev8qi_ssuss(__Int32x2_t, __Uint8x8_t, __Int8x8_t, int);
__Int32x4_t __builtin_aarch64_usdotv16qi_ssus(__Int32x4_t, __Uint8x16_t, __Int8x16_t);
__Int32x2_t __builtin_aarch64_usdotv8qi_ssus(__Int32x2_t, __Uint8x8_t, __Int8x8_t);
__Int8x16_t __builtin_aarch64_uzp1v16qi(__Int8x16_t, __Int8x16_t);
__Float64x2_t __builtin_aarch64_uzp1v2df(__Float64x2_t, __Float64x2_t);
__Int64x2_t __builtin_aarch64_uzp1v2di(__Int64x2_t, __Int64x2_t);
__Float32x2_t __builtin_aarch64_uzp1v2sf(__Float32x2_t, __Float32x2_t);
__Int32x2_t __builtin_aarch64_uzp1v2si(__Int32x2_t, __Int32x2_t);
__Int16x4_t __builtin_aarch64_uzp1v4hi(__Int16x4_t, __Int16x4_t);
__Float32x4_t __builtin_aarch64_uzp1v4sf(__Float32x4_t, __Float32x4_t);
__Int32x4_t __builtin_aarch64_uzp1v4si(__Int32x4_t, __Int32x4_t);
__Int16x8_t __builtin_aarch64_uzp1v8hi(__Int16x8_t, __Int16x8_t);
__Int8x8_t __builtin_aarch64_uzp1v8qi(__Int8x8_t, __Int8x8_t);
__Int8x16_t __builtin_aarch64_uzp2v16qi(__Int8x16_t, __Int8x16_t);
__Float64x2_t __builtin_aarch64_uzp2v2df(__Float64x2_t, __Float64x2_t);
__Int64x2_t __builtin_aarch64_uzp2v2di(__Int64x2_t, __Int64x2_t);
__Float32x2_t __builtin_aarch64_uzp2v2sf(__Float32x2_t, __Float32x2_t);
__Int32x2_t __builtin_aarch64_uzp2v2si(__Int32x2_t, __Int32x2_t);
__Int16x4_t __builtin_aarch64_uzp2v4hi(__Int16x4_t, __Int16x4_t);
__Float32x4_t __builtin_aarch64_uzp2v4sf(__Float32x4_t, __Float32x4_t);
__Int32x4_t __builtin_aarch64_uzp2v4si(__Int32x4_t, __Int32x4_t);
__Int16x8_t __builtin_aarch64_uzp2v8hi(__Int16x8_t, __Int16x8_t);
__Int8x8_t __builtin_aarch64_uzp2v8qi(__Int8x8_t, __Int8x8_t);
__Int64x2_t __builtin_aarch64_vec_smlal_laneq_v2si(__Int64x2_t, __Int32x2_t, __Int32x4_t, int);
__Int32x4_t __builtin_aarch64_vec_smlal_laneq_v4hi(__Int32x4_t, __Int16x4_t, __Int16x8_t, int);
__Int64x2_t __builtin_aarch64_vec_smlal_lane_v2si(__Int64x2_t, __Int32x2_t, __Int32x2_t, int);
__Int32x4_t __builtin_aarch64_vec_smlal_lane_v4hi(__Int32x4_t, __Int16x4_t, __Int16x4_t, int);
__Int64x2_t __builtin_aarch64_vec_smult_laneq_v2si(__Int32x2_t, __Int32x4_t, int);
__Int32x4_t __builtin_aarch64_vec_smult_laneq_v4hi(__Int16x4_t, __Int16x8_t, int);
__Int64x2_t __builtin_aarch64_vec_smult_lane_v2si(__Int32x2_t, __Int32x2_t, int);
__Int32x4_t __builtin_aarch64_vec_smult_lane_v4hi(__Int16x4_t, __Int16x4_t, int);
__Uint64x2_t __builtin_aarch64_vec_umlal_laneq_v2si_uuuus(__Uint64x2_t, __Uint32x2_t, __Uint32x4_t, int);
__Uint32x4_t __builtin_aarch64_vec_umlal_laneq_v4hi_uuuus(__Uint32x4_t, __Uint16x4_t, __Uint16x8_t, int);
__Uint64x2_t __builtin_aarch64_vec_umlal_lane_v2si_uuuus(__Uint64x2_t, __Uint32x2_t, __Uint32x2_t, int);
__Uint32x4_t __builtin_aarch64_vec_umlal_lane_v4hi_uuuus(__Uint32x4_t, __Uint16x4_t, __Uint16x4_t, int);
__Uint64x2_t __builtin_aarch64_vec_umult_laneq_v2si_uuus(__Uint32x2_t, __Uint32x4_t, int);
__Uint32x4_t __builtin_aarch64_vec_umult_laneq_v4hi_uuus(__Uint16x4_t, __Uint16x8_t, int);
__Uint64x2_t __builtin_aarch64_vec_umult_lane_v2si_uuus(__Uint32x2_t, __Uint32x2_t, int);
__Uint32x4_t __builtin_aarch64_vec_umult_lane_v4hi_uuus(__Uint16x4_t, __Uint16x4_t, int);
__Int16x8_t __builtin_aarch64_vec_widen_smult_hi_v16qi(__Int8x16_t, __Int8x16_t);
__Int64x2_t __builtin_aarch64_vec_widen_smult_hi_v4si(__Int32x4_t, __Int32x4_t);
__Int32x4_t __builtin_aarch64_vec_widen_smult_hi_v8hi(__Int16x8_t, __Int16x8_t);
__Uint16x8_t __builtin_aarch64_vec_widen_umult_hi_v16qi_uuu(__Uint8x16_t, __Uint8x16_t);
__Uint64x2_t __builtin_aarch64_vec_widen_umult_hi_v4si_uuu(__Uint32x4_t, __Uint32x4_t);
__Uint32x4_t __builtin_aarch64_vec_widen_umult_hi_v8hi_uuu(__Uint16x8_t, __Uint16x8_t);
void* __builtin_aarch64_xpaclri(void*);
__Int8x16_t __builtin_aarch64_zip1v16qi(__Int8x16_t, __Int8x16_t);
__Float64x2_t __builtin_aarch64_zip1v2df(__Float64x2_t, __Float64x2_t);
__Int64x2_t __builtin_aarch64_zip1v2di(__Int64x2_t, __Int64x2_t);
__Float32x2_t __builtin_aarch64_zip1v2sf(__Float32x2_t, __Float32x2_t);
__Int32x2_t __builtin_aarch64_zip1v2si(__Int32x2_t, __Int32x2_t);
__Int16x4_t __builtin_aarch64_zip1v4hi(__Int16x4_t, __Int16x4_t);
__Float32x4_t __builtin_aarch64_zip1v4sf(__Float32x4_t, __Float32x4_t);
__Int32x4_t __builtin_aarch64_zip1v4si(__Int32x4_t, __Int32x4_t);
__Int16x8_t __builtin_aarch64_zip1v8hi(__Int16x8_t, __Int16x8_t);
__Int8x8_t __builtin_aarch64_zip1v8qi(__Int8x8_t, __Int8x8_t);
__Int8x16_t __builtin_aarch64_zip2v16qi(__Int8x16_t, __Int8x16_t);
__Float64x2_t __builtin_aarch64_zip2v2df(__Float64x2_t, __Float64x2_t);
__Int64x2_t __builtin_aarch64_zip2v2di(__Int64x2_t, __Int64x2_t);
__Float32x2_t __builtin_aarch64_zip2v2sf(__Float32x2_t, __Float32x2_t);
__Int32x2_t __builtin_aarch64_zip2v2si(__Int32x2_t, __Int32x2_t);
__Int16x4_t __builtin_aarch64_zip2v4hi(__Int16x4_t, __Int16x4_t);
__Float32x4_t __builtin_aarch64_zip2v4sf(__Float32x4_t, __Float32x4_t);
__Int32x4_t __builtin_aarch64_zip2v4si(__Int32x4_t, __Int32x4_t);
__Int16x8_t __builtin_aarch64_zip2v8hi(__Int16x8_t, __Int16x8_t);
__Int8x8_t __builtin_aarch64_zip2v8qi(__Int8x8_t, __Int8x8_t);

// -march=armv8.5-a+memtag
void* __builtin_aarch64_memtag_get_tag(void*);
long unsigned int __builtin_aarch64_memtag_gmi(void*, long unsigned int);
void* __builtin_aarch64_memtag_inc_tag(void*, unsigned int);
void* __builtin_aarch64_memtag_irg(void*, long unsigned int);
void __builtin_aarch64_memtag_set_tag(void*);
long int __builtin_aarch64_memtag_subp(void*, void*);
// -march=armv8.5-a+tme
void __builtin_aarch64_tcancel(long unsigned int);
void __builtin_aarch64_tcommit(void);
long unsigned int __builtin_aarch64_tstart(void);
long unsigned int __builtin_aarch64_ttest(void);

#endif

#ifdef __IWMMXT__
typedef unsigned long long __mmx_m64;
typedef int __mmx_v2si __attribute__ ((vector_size (8)));
typedef short __mmx_v4hi __attribute__ ((vector_size (8)));

#ifdef __ANDROID__
typedef char __mmx_v8qi __attribute__ ((vector_size (8)));
__mmx_m64 __builtin_arm_wsadb (__mmx_v8qi  __A, __mmx_v8qi __B);
__mmx_m64 __builtin_arm_wsadh (__mmx_v4hi  __A, __mmx_v4hi __B);
void __builtin_arm_setwcx(int x, int y);
int __builtin_arm_getwcx(int x);
#else
typedef signed char __mmx_v8qi __attribute__ ((vector_size (8)));
__mmx_m64 __builtin_arm_wsadb (__mmx_v2si  __A, __mmx_v8qi __B, __mmx_v8qi __C);
__mmx_m64 __builtin_arm_wsadh (__mmx_v2si  __A, __mmx_v4hi __B, __mmx_v4hi __C);
#endif // __ANDROID__

__mmx_m64 __builtin_arm_walign (__mmx_v8qi __a, __mmx_v8qi __b, int __C);
__mmx_m64 __builtin_arm_wpackhss (__mmx_v4hi __m1, __mmx_v4hi __m2);
__mmx_m64 __builtin_arm_wpackwss (__mmx_v2si  __m1, __mmx_v2si  __m2);
__mmx_m64 __builtin_arm_wpackdss (long long __m1, long long __m2);
__mmx_m64 __builtin_arm_wpackhus (__mmx_v4hi __m1, __mmx_v4hi __m2);
__mmx_m64 __builtin_arm_wpackwus (__mmx_v2si  __m1, __mmx_v2si  __m2);
__mmx_m64 __builtin_arm_wpackdus (long long __m1, long long __m2);
__mmx_m64 __builtin_arm_wunpckihb (__mmx_v8qi __m1, __mmx_v8qi __m2);
__mmx_m64 __builtin_arm_wunpckihh (__mmx_v4hi __m1, __mmx_v4hi __m2);
__mmx_m64 __builtin_arm_wunpckihw (__mmx_v2si  __m1, __mmx_v2si  __m2);
__mmx_m64 __builtin_arm_wunpckilb (__mmx_v8qi __m1, __mmx_v8qi __m2);
__mmx_m64 __builtin_arm_wunpckilh (__mmx_v4hi __m1, __mmx_v4hi __m2);
__mmx_m64 __builtin_arm_wunpckilw (__mmx_v2si  __m1, __mmx_v2si  __m2);
__mmx_m64 __builtin_arm_wunpckelsb (__mmx_v8qi __m1);
__mmx_m64 __builtin_arm_wunpckelsh (__mmx_v4hi __m1);
__mmx_m64 __builtin_arm_wunpckelsw (__mmx_v2si  __m1);
__mmx_m64 __builtin_arm_wunpckehsb (__mmx_v8qi __m1);
__mmx_m64 __builtin_arm_wunpckehsh (__mmx_v4hi __m1);
__mmx_m64 __builtin_arm_wunpckehsw (__mmx_v2si  __m1);
__mmx_m64 __builtin_arm_wunpckelub (__mmx_v8qi __m1);
__mmx_m64 __builtin_arm_wunpckeluh (__mmx_v4hi __m1);
__mmx_m64 __builtin_arm_wunpckeluw (__mmx_v2si  __m1);
__mmx_m64 __builtin_arm_wunpckehub (__mmx_v8qi __m1);
__mmx_m64 __builtin_arm_wunpckehuh (__mmx_v4hi __m1);
__mmx_m64 __builtin_arm_wunpckehuw (__mmx_v2si  __m1);
__mmx_m64 __builtin_arm_waddb (__mmx_v8qi __m1, __mmx_v8qi __m2);
__mmx_m64 __builtin_arm_waddh (__mmx_v4hi __m1, __mmx_v4hi __m2);
__mmx_m64 __builtin_arm_waddw (__mmx_v2si  __m1, __mmx_v2si  __m2);
__mmx_m64 __builtin_arm_waddbss (__mmx_v8qi __m1, __mmx_v8qi __m2);
__mmx_m64 __builtin_arm_waddhss (__mmx_v4hi __m1, __mmx_v4hi __m2);
__mmx_m64 __builtin_arm_waddwss (__mmx_v2si  __m1, __mmx_v2si  __m2);
__mmx_m64 __builtin_arm_waddbus (__mmx_v8qi __m1, __mmx_v8qi __m2);
__mmx_m64 __builtin_arm_waddhus (__mmx_v4hi __m1, __mmx_v4hi __m2);
__mmx_m64 __builtin_arm_waddwus (__mmx_v2si  __m1, __mmx_v2si  __m2);
__mmx_m64 __builtin_arm_wsubb (__mmx_v8qi __m1, __mmx_v8qi __m2);
__mmx_m64 __builtin_arm_wsubh (__mmx_v4hi __m1, __mmx_v4hi __m2);
__mmx_m64 __builtin_arm_wsubw (__mmx_v2si  __m1, __mmx_v2si  __m2);
__mmx_m64 __builtin_arm_wsubbss (__mmx_v8qi __m1, __mmx_v8qi __m2);
__mmx_m64 __builtin_arm_wsubhss (__mmx_v4hi __m1, __mmx_v4hi __m2);
__mmx_m64 __builtin_arm_wsubwss (__mmx_v2si  __m1, __mmx_v2si  __m2);
__mmx_m64 __builtin_arm_wsubbus (__mmx_v8qi __m1, __mmx_v8qi __m2);
__mmx_m64 __builtin_arm_wsubhus (__mmx_v4hi __m1, __mmx_v4hi __m2);
__mmx_m64 __builtin_arm_wsubwus (__mmx_v2si  __m1, __mmx_v2si  __m2);
__mmx_m64 __builtin_arm_wmadds (__mmx_v4hi __m1, __mmx_v4hi __m2);
__mmx_m64 __builtin_arm_wmaddu (__mmx_v4hi __m1, __mmx_v4hi __m2);
__mmx_m64 __builtin_arm_wmulsm (__mmx_v4hi __m1, __mmx_v4hi __m2);
__mmx_m64 __builtin_arm_wmulum (__mmx_v4hi __m1, __mmx_v4hi __m2);
__mmx_m64 __builtin_arm_wmulul (__mmx_v4hi __m1, __mmx_v4hi __m2);
__mmx_m64 __builtin_arm_wsllh (__mmx_v4hi __m, int __count);
__mmx_m64 __builtin_arm_wsllhi (__mmx_v4hi __m, int __count);
__mmx_m64 __builtin_arm_wsllw (__mmx_v2si  __m, int __count);
__mmx_m64 __builtin_arm_wsllwi (__mmx_v2si  __m, int __count);
__mmx_m64 __builtin_arm_wslld (int __m, int __count);
__mmx_m64 __builtin_arm_wslldi (int __m, int __count);
__mmx_m64 __builtin_arm_wsrah (__mmx_v4hi __m, int __count);
__mmx_m64 __builtin_arm_wsrahi (__mmx_v4hi __m, int __count);
__mmx_m64 __builtin_arm_wsraw (__mmx_v2si  __m, int __count);
__mmx_m64 __builtin_arm_wsrawi (__mmx_v2si  __m, int __count);
__mmx_m64 __builtin_arm_wsrad (int __m, int __count);
__mmx_m64 __builtin_arm_wsradi (int __m, int __count);
__mmx_m64 __builtin_arm_wsrlh (__mmx_v4hi __m, int __count);
__mmx_m64 __builtin_arm_wsrlhi (__mmx_v4hi __m, int __count);
__mmx_m64 __builtin_arm_wsrlw (__mmx_v2si  __m, int __count);
__mmx_m64 __builtin_arm_wsrlwi (__mmx_v2si  __m, int __count);
__mmx_m64 __builtin_arm_wsrld (int __m, int __count);
__mmx_m64 __builtin_arm_wsrldi (int __m, int __count);
__mmx_m64 __builtin_arm_wrorh (__mmx_v4hi __m, int __count);
__mmx_m64 __builtin_arm_wrorhi (__mmx_v4hi __m, int __count);
__mmx_m64 __builtin_arm_wrorw (__mmx_v2si  __m, int __count);
__mmx_m64 __builtin_arm_wrorwi (__mmx_v2si  __m, int __count);
__mmx_m64 __builtin_arm_wrord (int __m, int __count);
__mmx_m64 __builtin_arm_wrordi (int __m, int __count);
__mmx_m64 __builtin_arm_wand (int __m1, int __m2);
__mmx_m64 __builtin_arm_wandn (int __m2, int __m1);
__mmx_m64 __builtin_arm_wor (int __m1, int __m2);
__mmx_m64 __builtin_arm_wxor (int __m1, int __m2);
__mmx_m64 __builtin_arm_wcmpeqb (__mmx_v8qi __m1, __mmx_v8qi __m2);
__mmx_m64 __builtin_arm_wcmpgtsb (__mmx_v8qi __m1, __mmx_v8qi __m2);
__mmx_m64 __builtin_arm_wcmpgtub (__mmx_v8qi __m1, __mmx_v8qi __m2);
__mmx_m64 __builtin_arm_wcmpeqh (__mmx_v4hi __m1, __mmx_v4hi __m2);
__mmx_m64 __builtin_arm_wcmpgtsh (__mmx_v4hi __m1, __mmx_v4hi __m2);
__mmx_m64 __builtin_arm_wcmpgtuh (__mmx_v4hi __m1, __mmx_v4hi __m2);
__mmx_m64 __builtin_arm_wcmpeqw (__mmx_v2si  __m1, __mmx_v2si  __m2);
__mmx_m64 __builtin_arm_wcmpgtsw (__mmx_v2si  __m1, __mmx_v2si  __m2);
__mmx_m64 __builtin_arm_wcmpgtuw (__mmx_v2si  __m1, __mmx_v2si  __m2);
__mmx_m64 __builtin_arm_wmacu (int __A, __mmx_v4hi __B, __mmx_v4hi __C);
__mmx_m64 __builtin_arm_wmacs (int __A, __mmx_v4hi __B, __mmx_v4hi __C);
__mmx_m64 __builtin_arm_wmacuz (__mmx_v4hi __A, __mmx_v4hi __B);
__mmx_m64 __builtin_arm_wmacsz (__mmx_v4hi __A, __mmx_v4hi __B);
__mmx_m64 __builtin_arm_waccb (__mmx_v8qi __A);
__mmx_m64 __builtin_arm_wacch (__mmx_v4hi __A);
__mmx_m64 __builtin_arm_waccw (__mmx_v2si  __A);
__mmx_m64 __builtin_arm_tmia (int __A, int __B, int __C);
__mmx_m64 __builtin_arm_tmiaph (int __A, int __B, int __C);
__mmx_m64 __builtin_arm_tmiabb (int __A, int __B, int __C);
__mmx_m64 __builtin_arm_tmiabt (int __A, int __B, int __C);
__mmx_m64 __builtin_arm_tmiatb (int __A, int __B, int __C);
__mmx_m64 __builtin_arm_tmiatt (int __A, int __B, int __C);
__mmx_m64 __builtin_arm_wmaxsb (__mmx_v8qi __A, __mmx_v8qi __B);
__mmx_m64 __builtin_arm_wmaxsh (__mmx_v4hi __A, __mmx_v4hi __B);
__mmx_m64 __builtin_arm_wmaxsw (__mmx_v2si  __A, __mmx_v2si  __B);
__mmx_m64 __builtin_arm_wmaxub (__mmx_v8qi __A, __mmx_v8qi __B);
__mmx_m64 __builtin_arm_wmaxuh (__mmx_v4hi __A, __mmx_v4hi __B);
__mmx_m64 __builtin_arm_wmaxuw (__mmx_v2si  __A, __mmx_v2si  __B);
__mmx_m64 __builtin_arm_wminsb (__mmx_v8qi __A, __mmx_v8qi __B);
__mmx_m64 __builtin_arm_wminsh (__mmx_v4hi __A, __mmx_v4hi __B);
__mmx_m64 __builtin_arm_wminsw (__mmx_v2si  __A, __mmx_v2si  __B);
__mmx_m64 __builtin_arm_wminub (__mmx_v8qi __A, __mmx_v8qi __B);
__mmx_m64 __builtin_arm_wminuh (__mmx_v4hi __A, __mmx_v4hi __B);
__mmx_m64 __builtin_arm_wminuw (__mmx_v2si  __A, __mmx_v2si  __B);
__mmx_m64 __builtin_arm_tmovmskb (__mmx_v8qi __A);
__mmx_m64 __builtin_arm_tmovmskh (__mmx_v4hi __A);
__mmx_m64 __builtin_arm_tmovmskw (__mmx_v2si  __A);
__mmx_m64 __builtin_arm_wavg2br (__mmx_v8qi __A, __mmx_v8qi __B);
__mmx_m64 __builtin_arm_wavg2hr (__mmx_v4hi __A, __mmx_v4hi __B);
__mmx_m64 __builtin_arm_wavg2b (__mmx_v8qi __A, __mmx_v8qi __B);
__mmx_m64 __builtin_arm_wavg2h (__mmx_v4hi __A, __mmx_v4hi __B);
__mmx_m64 __builtin_arm_wsadbz (__mmx_v8qi __A, __mmx_v8qi __B);
__mmx_m64 __builtin_arm_wsadhz (__mmx_v4hi __A, __mmx_v4hi __B);
__mmx_m64 __builtin_arm_wsadbz (__mmx_v8qi __A, __mmx_v8qi __B);
__mmx_m64 __builtin_arm_wsadhz (__mmx_v4hi __A, __mmx_v4hi __B);
__mmx_m64 __builtin_arm_wzero ();
__mmx_m64 __builtin_arm_setwcgr0 (int __value);
__mmx_m64 __builtin_arm_setwcgr1 (int __value);
__mmx_m64 __builtin_arm_setwcgr2 (int __value);
__mmx_m64 __builtin_arm_setwcgr3 (int __value);
__mmx_m64 __builtin_arm_getwcgr0 ();
__mmx_m64 __builtin_arm_getwcgr1 ();
__mmx_m64 __builtin_arm_getwcgr2 ();
__mmx_m64 __builtin_arm_getwcgr3 ();
__mmx_m64 __builtin_arm_wabsb (__mmx_v8qi m1);
__mmx_m64 __builtin_arm_wabsh (__mmx_v4hi __m1);
__mmx_m64 __builtin_arm_wabsw (__mmx_v2si  __m1);
__mmx_m64 __builtin_arm_waddsubhx (__mmx_v4hi __a, __mmx_v4hi __b);
__mmx_m64 __builtin_arm_wabsdiffb (__mmx_v8qi __a, __mmx_v8qi __b);
__mmx_m64 __builtin_arm_wabsdiffh (__mmx_v4hi __a, __mmx_v4hi __b);
__mmx_m64 __builtin_arm_wabsdiffw (__mmx_v2si __a, __mmx_v2si __b);
__mmx_m64 __builtin_arm_wavg4 (__mmx_v8qi __a, __mmx_v8qi __b);
__mmx_m64 __builtin_arm_wavg4r (__mmx_v8qi __a, __mmx_v8qi __b);
__mmx_m64 __builtin_arm_wmaddsx (__mmx_v4hi __a, __mmx_v4hi __b);
__mmx_m64 __builtin_arm_wmaddux (__mmx_v4hi __a, __mmx_v4hi __b);
__mmx_m64 __builtin_arm_wmaddsn (__mmx_v4hi __a, __mmx_v4hi __b);
__mmx_m64 __builtin_arm_wmaddun (__mmx_v4hi __a, __mmx_v4hi __b);
__mmx_m64 __builtin_arm_wmulwsm (__mmx_v2si __a, __mmx_v2si __b);
__mmx_m64 __builtin_arm_wmulwum (__mmx_v2si __a, __mmx_v2si __b);
__mmx_m64 __builtin_arm_wmulsmr (__mmx_v4hi __a, __mmx_v4hi __b);
__mmx_m64 __builtin_arm_wmulwsmr (__mmx_v2si __a, __mmx_v2si __b);
__mmx_m64 __builtin_arm_wmulumr (__mmx_v4hi __a, __mmx_v4hi __b);
__mmx_m64 __builtin_arm_wmulwumr (__mmx_v2si __a, __mmx_v2si __b);
__mmx_m64 __builtin_arm_wmulwl (__mmx_v2si __a, __mmx_v2si __b);
__mmx_m64 __builtin_arm_wqmulm (__mmx_v4hi __a, __mmx_v4hi __b);
__mmx_m64 __builtin_arm_wqmulwm (__mmx_v2si __a, __mmx_v2si __b);
__mmx_m64 __builtin_arm_wqmulmr (__mmx_v4hi __a, __mmx_v4hi __b);
__mmx_m64 __builtin_arm_wqmulwmr (__mmx_v2si __a, __mmx_v2si __b);
__mmx_m64 __builtin_arm_wsubaddhx (__mmx_v4hi __a, __mmx_v4hi __b);
__mmx_m64 __builtin_arm_waddbhusl (__mmx_v4hi __a, __mmx_v8qi __b);
__mmx_m64 __builtin_arm_waddbhusm (__mmx_v4hi __a, __mmx_v8qi __b);
__mmx_m64 __builtin_arm_walignr0 (__mmx_v8qi __a, __mmx_v8qi __b);
__mmx_m64 __builtin_arm_walignr1 (__mmx_v8qi __a, __mmx_v8qi __b);
__mmx_m64 __builtin_arm_walignr2 (__mmx_v8qi __a, __mmx_v8qi __b);
__mmx_m64 __builtin_arm_walignr3 (__mmx_v8qi __a, __mmx_v8qi __b);
__mmx_m64 __builtin_arm_tbcstb (signed char value);
__mmx_m64 __builtin_arm_tbcsth (short value);
__mmx_m64 __builtin_arm_tbcstw (int value);
int __builtin_arm_textrmsb(__mmx_v8qi, int);
int __builtin_arm_textrmsh(__mmx_v4hi, int);
int __builtin_arm_textrmsw(__mmx_v2si, int);
int __builtin_arm_textrmub(__mmx_v8qi, int);
int __builtin_arm_textrmuh(___mmx_v4hi, int);
int __builtin_arm_textrmuw(__mmx_v2si, int);
__mmx_v8qi __builtin_arm_tinsrb(__mmx_v8qi, int, int);
___mmx_v4hi __builtin_arm_tinsrh(___mmx_v4hi, int, int);
__mmx_v2si __builtin_arm_tinsrw(__mmx_v2si, int, int);
___mmx_v4hi __builtin_arm_wshufh(___mmx_v4hi, int);
#endif // __IWMMX__

#ifdef __IWMMXT2__ //enable with -march=iwmmxt2
long long unsigned int __builtin_arm_wmerge(long long unsigned int, long long unsigned int, int);
long long unsigned int __builtin_arm_wmiabb(long long unsigned int, __mmx_v4hi, __mmx_v4hi);
long long unsigned int __builtin_arm_wmiabbn(long long unsigned int, __mmx_v4hi, __mmx_v4hi);
long long unsigned int __builtin_arm_wmiabt(long long unsigned int, __mmx_v4hi, __mmx_v4hi);
long long unsigned int __builtin_arm_wmiabtn(long long unsigned int, __mmx_v4hi, __mmx_v4hi);
long long unsigned int __builtin_arm_wmiatb(long long unsigned int, __mmx_v4hi, __mmx_v4hi);
long long unsigned int __builtin_arm_wmiatbn(long long unsigned int, __mmx_v4hi, __mmx_v4hi);
long long unsigned int __builtin_arm_wmiatt(long long unsigned int, __mmx_v4hi, __mmx_v4hi);
long long unsigned int __builtin_arm_wmiattn(long long unsigned int, __mmx_v4hi, __mmx_v4hi);
long long unsigned int __builtin_arm_wmiawbb(long long unsigned int, __mmx_v2si, __mmx_v2si);
long long unsigned int __builtin_arm_wmiawbbn(long long unsigned int, __mmx_v2si, __mmx_v2si);
long long unsigned int __builtin_arm_wmiawbt(long long unsigned int, __mmx_v2si, __mmx_v2si);
long long unsigned int __builtin_arm_wmiawbtn(long long unsigned int, __mmx_v2si, __mmx_v2si);
long long unsigned int __builtin_arm_wmiawtb(long long unsigned int, __mmx_v2si, __mmx_v2si);
long long unsigned int __builtin_arm_wmiawtbn(long long unsigned int, __mmx_v2si, __mmx_v2si);
long long unsigned int __builtin_arm_wmiawtt(long long unsigned int, __mmx_v2si, __mmx_v2si);
long long unsigned int __builtin_arm_wmiawttn(long long unsigned int, __mmx_v2si, __mmx_v2si);
__mmx_v2si __builtin_arm_wqmiabb(__mmx_v2si, __mmx_v4hi, __mmx_v4hi);
__mmx_v2si __builtin_arm_wqmiabbn(__mmx_v2si, __mmx_v4hi, __mmx_v4hi);
__mmx_v2si __builtin_arm_wqmiabt(__mmx_v2si, __mmx_v4hi, __mmx_v4hi);
__mmx_v2si __builtin_arm_wqmiabtn(__mmx_v2si, __mmx_v4hi, __mmx_v4hi);
__mmx_v2si __builtin_arm_wqmiatb(__mmx_v2si, __mmx_v4hi, __mmx_v4hi);
__mmx_v2si __builtin_arm_wqmiatbn(__mmx_v2si, __mmx_v4hi, __mmx_v4hi);
__mmx_v2si __builtin_arm_wqmiatt(__mmx_v2si, __mmx_v4hi, __mmx_v4hi);
__mmx_v2si __builtin_arm_wqmiattn(__mmx_v2si, __mmx_v4hi, __mmx_v4hi);
#endif

#ifdef __ARM_FP16_ARGS // arm-eabi -mfp16-format=ieee or aarch64
typedef float cov_fp16 __attribute__((mode(HF)));

cov_fp16 __builtin_ceilf16(cov_fp16);
cov_fp16 __builtin_copysignf16(cov_fp16, cov_fp16);
cov_fp16 __builtin_fabsf16(cov_fp16);
cov_fp16 __builtin_floorf16(cov_fp16);
cov_fp16 __builtin_fmaf16(cov_fp16, cov_fp16, cov_fp16);
cov_fp16 __builtin_fmaxf16(cov_fp16, cov_fp16);
cov_fp16 __builtin_fminf16(cov_fp16, cov_fp16);
cov_fp16 __builtin_nanf16(const char*);
cov_fp16 __builtin_nansf16(const char*);
cov_fp16 __builtin_nearbyintf16(cov_fp16);
cov_fp16 __builtin_rintf16(cov_fp16);
cov_fp16 __builtin_roundevenf16(cov_fp16);
cov_fp16 __builtin_roundf16(cov_fp16);
cov_fp16 __builtin_sqrtf16(cov_fp16);
cov_fp16 __builtin_truncf16(cov_fp16);
cov_fp16 __builtin_huge_valf16(void);
cov_fp16 __builtin_inff16(void);
#endif

#ifdef __coverity_cplusplus
}
#endif
/*
  Copyright (c) 2023 Synopsys, Inc. All rights reserved worldwide.
  The information contained in this file is the proprietary and confidential
  information of Synopsys, Inc. and its licensors, and is supplied subject to,
  and may be used only by Synopsys customers in accordance with the terms and
  conditions of a previously executed license agreement between Synopsys and
  that customer.
*/


#define __COVERITY_GCC_VERSION_AT_LEAST(maj, min) \
    ((__GNUC__ > (maj)) || (__GNUC__ == (maj) && __GNUC_MINOR__ >= (min)))

#if __COVERITY_GCC_VERSION_AT_LEAST(4, 9)
// Starting with GCC 4.9, instruction set intrinsics are always visible
// regardless of whether or not the instruction set is enabled.
#define __COVERITY_GCC49_INTRINSICS 1
#else // GCC <4.9
#define __COVERITY_GCC49_INTRINSICS 0
#endif

#ifdef __IA64__
typedef __coverity___fpreg long double __fpreg;
#endif


#ifdef __coverity_cplusplus
extern "C" {
#endif

#if defined(__SIZEOF_FLOAT128__) && defined(__coverity_cplusplus)
#define cov_f128_t __float128
#else
#define cov_f128_t long double
#endif

// The following macros are used in the Linux Kernel
#if defined(__linux__) && defined(__KERNEL__)
#nodef BUG_ON(x) do { if (x) __coverity_panic__(); } while (0)
#nodef WARN_ON(x) ({ int result = !!(x); if (result) __coverity_panic__(); result; })
#nodef BUG() __coverity_panic__()
void __coverity_panic__();
#endif // Linux kernel

#nodef setjmp
int setjmp(void *);

#ifndef __COVERITY_NO_STRING_NODEFS__
// Function list obtained from "cstring"+memrchr+stpcopy (from bits/string.h)

#nodef memcpy
void *memcpy(void *, const void *, __COVERITY_SIZE_TYPE__);

#nodef memmove
void *memmove(void *, const void *, __COVERITY_SIZE_TYPE__);

#nodef strcpy
char *strcpy(char *, const char *);

#nodef strncpy
char *strncpy(char *, const char *, __COVERITY_SIZE_TYPE__);

#nodef strcat
char *strcat(char *, const char *);

#nodef strncat
char *strncat(char *, const char *, __COVERITY_SIZE_TYPE__);

#nodef memcmp
int memcmp(const void *, const void *, __COVERITY_SIZE_TYPE__ n);

#nodef strcmp
int strcmp(const char *, const char *);

#nodef strcoll
int strcoll(const char *, const char *);

#nodef strncmp
int strncmp(const char *, const char *, __COVERITY_SIZE_TYPE__);

#nodef strxfrm
__COVERITY_SIZE_TYPE__
strxfrm(char *, const char *, __COVERITY_SIZE_TYPE__);

#nodef memchr
void *memchr(const void *, int, __COVERITY_SIZE_TYPE__);

#nodef strchr
char *strchr(const char *, int);

#nodef strcspn
__COVERITY_SIZE_TYPE__ strcspn(const char *, const char *);

#nodef strpbrk
char *strpbrk(const char *, const char *);

#nodef strrchr
char *strrchr(const char *, int);

#nodef strspn
__COVERITY_SIZE_TYPE__ strspn(const char *, const char *);

#nodef strstr
char *strstr(const char *, const char *);

#nodef strtok
char *strtok(char *, const char *);

#nodef memset
void *memset(void *, int, __COVERITY_SIZE_TYPE__);

#nodef strlen
__COVERITY_SIZE_TYPE__ strlen(const char *);

#nodef strerror
char *strerror(int);

#nodef memrchr
void *memrchr(const void *, int, __COVERITY_SIZE_TYPE__);

#nodef stpcpy
char *stpcpy(char *, const char *);

#nodef strdup
char *strdup(const char *);
#endif // __COVERITY_NO_STRING_NODEFS__

#define __builtin_expect_with_probability(expr, value, probability) (expr)
#define __builtin_speculation_safe_value(t1, t2) (t1)

#ifndef __coverity_cplusplus
#if __COVERITY_GCC_VERSION_AT_LEAST(7, 0)
typedef float __coverity_float _Float32;
typedef double __coverity_floatx _Float32x;
typedef double __coverity_float _Float64;
typedef long double _Float64x;
typedef long double _Float128;
#endif /* GCC >= 7.0 */
#endif /* __coverity_cplusplus */

#ifdef __coverity_cplusplus
}
#endif
#if __COVERITY_GCC_VERSION_AT_LEAST(4, 0)
typedef float __coverity_decimal _Decimal32;
typedef double __coverity_decimal _Decimal64;
typedef long double __coverity_decimal _Decimal128;

_Decimal32 __builtin_nand32(const char*);
_Decimal32 __builtin_infd32();
_Decimal64 __builtin_infd64();
_Decimal64 __builtin_nand64(const char*);
_Decimal128 __builtin_infd128();
_Decimal128 __builtin_nand128(const char*);
_Decimal128 __builtin_fabsd128(_Decimal128);
_Decimal32 __builtin_fabsd32(_Decimal32);
_Decimal64 __builtin_fabsd64(_Decimal64);
int __builtin_finited128(_Decimal128);
int __builtin_finited32(_Decimal32);
int __builtin_finited64(_Decimal64);
int __builtin_isinfd128(_Decimal128);
int __builtin_isinfd32(_Decimal32);
int __builtin_isinfd64(_Decimal64);
int __builtin_isnand128(_Decimal128);
int __builtin_isnand32(_Decimal32);
int __builtin_isnand64(_Decimal64);
int __builtin_signbitd128(_Decimal128);
int __builtin_signbitd32(_Decimal32);
int __builtin_signbitd64(_Decimal64);
_Decimal128 __builtin_nansd128(const char*);
_Decimal32 __builtin_nansd32(const char*);
_Decimal64 __builtin_nansd64(const char*);
#endif /* GCC >= 4.0 */

#ifdef __COVERITY_CILKPLUS
#define _Cilk_spawn
#define _Cilk_sync
#define _Cilk_for for
#endif /* __cilk */

#if __COVERITY_GCC_VERSION_AT_LEAST(8, 0)
/*
 * The following macros are used to implement __builtin_tgmath. This GCC
 * intrinsic is used to support type generic math operations in C modes. These
 * can be disabled by defining __COVERITY_DISABLE_TGTMATH.
 */
#if !defined(__COVERITY_DISABLE_TGTMATH) && !defined(__coverity_cplusplus)
#define __coverity_count_args(...) __coverity_count_args_n(-,##__VA_ARGS__,9,8,7,6,5,4,3,2,1,0)
#define __coverity_count_args_n(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, count, ...) count
#define __coverity_cat(x, ...) __coverity_cat_n(x, __VA_ARGS__)
#define __coverity_cat_n(x, ...) x ## __VA_ARGS__

#define __builtin_tgmath(...) \
    __coverity_cat(__coverity_tgmath_,__coverity_count_args(__VA_ARGS__))(__VA_ARGS__)

#define __coverity_tgmath_4(...) __coverity_tgmath_real(__VA_ARGS__)
#define __coverity_tgmath_5(...) __coverity_tgmath_real(__VA_ARGS__)
#define __coverity_tgmath_6(...) __coverity_tgmath_real(__VA_ARGS__)
#define __coverity_tgmath_7(...) __coverity_tgmath_cplx(__VA_ARGS__)
#define __coverity_tgmath_8(...) __coverity_tgmath_cplx(__VA_ARGS__)
#define __coverity_tgmath_9(...) __coverity_tgmath_cplx(__VA_ARGS__)

#define __coverity_tgmath_real(rf, r, rl, arg0, ...) \
    _Generic((arg0), \
        float: (rf)(arg0, ## __VA_ARGS__), \
        long double: (rl)(arg0, ## __VA_ARGS__), \
        default: (r)(arg0, ## __VA_ARGS__))
#define __coverity_tgmath_cplx(rf, r, rl, cf, c, cl, arg0, ...) \
    _Generic((arg0), \
        float: (rf)(arg0, ## __VA_ARGS__), \
        long double: (rl)(arg0, ## __VA_ARGS__), \
        __complex__ float: (cf)(arg0, ## __VA_ARGS__), \
        __complex__ long double: (cl)(arg0, ## __VA_ARGS__), \
        __complex__ double: (c)(arg0, ## __VA_ARGS__), \
        default: (r)(arg0, ## __VA_ARGS__))
#endif /* !__COVERITY_DISABLE_TGTMATH_DECLS && !__coverity_cplusplus */
#endif /* GCC 8+ */
/*
  Copyright (c) 2023 Synopsys, Inc. All rights reserved worldwide.
  The information contained in this file is the proprietary and confidential
  information of Synopsys, Inc. and its licensors, and is supplied subject to,
  and may be used only by Synopsys customers in accordance with the terms and
  conditions of a previously executed license agreement between Synopsys and
  that customer.
*/

/*
 * These declarations are needed for GCC's transactional memory support. They
 * are not supported intrinsically by cov-emit.
 */

// -fgnu-tm -mavx
void __builtin__ITM_abortTransaction(int);
unsigned int __builtin__ITM_beginTransaction(unsigned int, ...);
void* __builtin__ITM_calloc(long unsigned int, long unsigned int);
void __builtin__ITM_changeTransactionMode(int);
void __builtin__ITM_commitTransactionEH(void*);
void __builtin__ITM_free(void*);
void* __builtin__ITM_getTMCloneOrIrrevocable(void*);
void* __builtin__ITM_getTMCloneSafe(void*);
void __builtin__ITM_LB(volatile void*, long unsigned int);
void __builtin__ITM_LD(volatile void*);
void __builtin__ITM_LE(volatile void*);
void __builtin__ITM_LF(volatile void*);
void __builtin__ITM_LM128(const void*);
void __builtin__ITM_LM256(const void*);
void __builtin__ITM_LM64(const void*);
void __builtin__ITM_LU1(volatile void*);
void __builtin__ITM_LU2(volatile void*);
void __builtin__ITM_LU4(volatile void*);
void __builtin__ITM_LU8(volatile void*);
void* __builtin__ITM_malloc(long unsigned int);
void __builtin__ITM_memcpyRnWt(void*, const void*, long unsigned int);
void __builtin__ITM_memcpyRtWn(void*, const void*, long unsigned int);
void __builtin__ITM_memcpyRtWt(void*, const void*, long unsigned int);
void __builtin__ITM_memmoveRtWt(void*, const void*, long unsigned int);
void __builtin__ITM_memsetW(void*, int, long unsigned int);
double __builtin__ITM_RaRD(double*);
long double __builtin__ITM_RaRE(volatile void*);
float __builtin__ITM_RaRF(volatile void*);
__edg_vector_type__(float,4) __builtin__ITM_RaRM128(const __edg_vector_type__(float,4)*);
__edg_vector_type__(float,8) __builtin__ITM_RaRM256(const __edg_vector_type__(float,8)*);
__edg_vector_type__(int,2) __builtin__ITM_RaRM64(const __edg_vector_type__(int,2)*);
unsigned char __builtin__ITM_RaRU1(volatile void*);
short unsigned int __builtin__ITM_RaRU2(volatile void*);
unsigned int __builtin__ITM_RaRU4(volatile void*);
long unsigned int __builtin__ITM_RaRU8(volatile void*);
double __builtin__ITM_RaWD(double*);
long double __builtin__ITM_RaWE(volatile void*);
float __builtin__ITM_RaWF(volatile void*);
__edg_vector_type__(float,4) __builtin__ITM_RaWM128(const __edg_vector_type__(float,4)*);
__edg_vector_type__(float,8) __builtin__ITM_RaWM256(const __edg_vector_type__(float,8)*);
__edg_vector_type__(int,2) __builtin__ITM_RaWM64(const __edg_vector_type__(int,2)*);
unsigned char __builtin__ITM_RaWU1(volatile void*);
short unsigned int __builtin__ITM_RaWU2(volatile void*);
unsigned int __builtin__ITM_RaWU4(volatile void*);
long unsigned int __builtin__ITM_RaWU8(volatile void*);
double __builtin__ITM_RD(double*);
long double __builtin__ITM_RE(volatile void*);
float __builtin__ITM_RF(volatile void*);
double __builtin__ITM_RfWD(double*);
long double __builtin__ITM_RfWE(volatile void*);
float __builtin__ITM_RfWF(volatile void*);
__edg_vector_type__(float,4) __builtin__ITM_RfWM128(const __edg_vector_type__(float,4)*);
__edg_vector_type__(float,8) __builtin__ITM_RfWM256(const __edg_vector_type__(float,8)*);
__edg_vector_type__(int,2) __builtin__ITM_RfWM64(const __edg_vector_type__(int,2)*);
unsigned char __builtin__ITM_RfWU1(volatile void*);
short unsigned int __builtin__ITM_RfWU2(volatile void*);
unsigned int __builtin__ITM_RfWU4(volatile void*);
long unsigned int __builtin__ITM_RfWU8(volatile void*);
__edg_vector_type__(float,4) __builtin__ITM_RM128(const __edg_vector_type__(float,4)*);
__edg_vector_type__(float,8) __builtin__ITM_RM256(const __edg_vector_type__(float,8)*);
__edg_vector_type__(int,2) __builtin__ITM_RM64(const __edg_vector_type__(int,2)*);
unsigned char __builtin__ITM_RU1(volatile void*);
short unsigned int __builtin__ITM_RU2(volatile void*);
unsigned int __builtin__ITM_RU4(volatile void*);
long unsigned int __builtin__ITM_RU8(volatile void*);
void __builtin__ITM_WaRD(volatile void*, double);
void __builtin__ITM_WaRE(volatile void*, long double);
void __builtin__ITM_WaRF(volatile void*, float);
void __builtin__ITM_WaRM128(__edg_vector_type__(float,4)*, __edg_vector_type__(float,4));
void __builtin__ITM_WaRM256(__edg_vector_type__(float,8)*, __edg_vector_type__(float,8));
void __builtin__ITM_WaRM64(__edg_vector_type__(int,2)*, __edg_vector_type__(int,2));
void __builtin__ITM_WaRU1(volatile void*, unsigned char);
void __builtin__ITM_WaRU2(volatile void*, short unsigned int);
void __builtin__ITM_WaRU4(volatile void*, unsigned int);
void __builtin__ITM_WaRU8(volatile void*, long unsigned int);
void __builtin__ITM_WaWD(volatile void*, double);
void __builtin__ITM_WaWE(volatile void*, long double);
void __builtin__ITM_WaWF(volatile void*, float);
void __builtin__ITM_WaWM128(__edg_vector_type__(float,4)*, __edg_vector_type__(float,4));
void __builtin__ITM_WaWM256(__edg_vector_type__(float,8)*, __edg_vector_type__(float,8));
void __builtin__ITM_WaWM64(__edg_vector_type__(int,2)*, __edg_vector_type__(int,2));
void __builtin__ITM_WaWU1(volatile void*, unsigned char);
void __builtin__ITM_WaWU2(volatile void*, short unsigned int);
void __builtin__ITM_WaWU4(volatile void*, unsigned int);
void __builtin__ITM_WaWU8(volatile void*, long unsigned int);
void __builtin__ITM_WD(volatile void*, double);
void __builtin__ITM_WE(volatile void*, long double);
void __builtin__ITM_WF(volatile void*, float);
void __builtin__ITM_WM128(__edg_vector_type__(float,4)*, __edg_vector_type__(float,4));
void __builtin__ITM_WM256(__edg_vector_type__(float,8)*, __edg_vector_type__(float,8));
void __builtin__ITM_WM64(__edg_vector_type__(int,2)*, __edg_vector_type__(int,2));
void __builtin__ITM_WU1(volatile void*, unsigned char);
void __builtin__ITM_WU2(volatile void*, short unsigned int);
void __builtin__ITM_WU4(volatile void*, unsigned int);
void __builtin__ITM_WU8(volatile void*, long unsigned int);
void __builtin__ITM_commitTransaction(void);

/*
  Copyright (c) 2023 Synopsys, Inc. All rights reserved worldwide.
  The information contained in this file is the proprietary and confidential
  information of Synopsys, Inc. and its licensors, and is supplied subject to,
  and may be used only by Synopsys customers in accordance with the terms and
  conditions of a previously executed license agreement between Synopsys and
  that customer.
*/

/*
 * These declarations are needed for GCC's OpenMP/OpenACC support. They are not
 * supported intrinsically by cov-emit.
 */

#ifdef _OPENMP
int __builtin_acc_get_device_type(void);
void __builtin_GOACC_data_end(void);
void __builtin_GOMP_atomic_end(void);
void __builtin_GOMP_atomic_start(void);
void __builtin_GOMP_barrier(void);
__edg_bool_type__ __builtin_GOMP_barrier_cancel(void);
void __builtin_GOMP_critical_end(void);
void __builtin_GOMP_critical_start(void);
void __builtin_GOMP_loop_end(void);
__edg_bool_type__ __builtin_GOMP_loop_end_cancel(void);
void __builtin_GOMP_loop_end_nowait(void);
void __builtin_GOMP_ordered_end(void);
void __builtin_GOMP_ordered_start(void);
void __builtin_GOMP_sections_end(void);
__edg_bool_type__ __builtin_GOMP_sections_end_cancel(void);
void __builtin_GOMP_sections_end_nowait(void);
unsigned int __builtin_GOMP_sections_next(void);
void* __builtin_GOMP_single_copy_start(void);
__edg_bool_type__ __builtin_GOMP_single_start(void);
void __builtin_GOMP_target_end_data(void);
void __builtin_GOMP_taskgroup_end(void);
void __builtin_GOMP_taskgroup_start(void);
void __builtin_GOMP_taskwait(void);
void __builtin_GOMP_taskyield(void);
int __builtin_omp_get_num_teams(void);
int __builtin_omp_get_num_threads(void);
int __builtin_omp_get_team_num(void);
int __builtin_omp_get_thread_num(void);
void __builtin_GOACC_data_start(int, long unsigned int, void*, void*, void*);
void __builtin_GOACC_declare(int, long unsigned int, void*, void*, void*);
void __builtin_GOACC_enter_exit_data(int, long unsigned int, void*, void*, void*, int, int, ...);
void __builtin_GOACC_parallel_keyed(int, void (*)(void*), long unsigned int, void*, void*, void*, ...);
int __builtin_goacc_parlevel_id(int);
int __builtin_goacc_parlevel_size(int);
void __builtin_GOACC_update(int, long unsigned int, void*, void*, void*, int, int, ...);
void __builtin_GOACC_wait(int, int, ...);
__edg_bool_type__ __builtin_GOMP_cancel(int, __edg_bool_type__);
__edg_bool_type__ __builtin_GOMP_cancellation_point(int);
void __builtin_GOMP_critical_name_end(void**);
void __builtin_GOMP_critical_name_start(void**);
void __builtin_GOMP_doacross_post(void*);
void __builtin_GOMP_doacross_ull_post(void*);
void __builtin_GOMP_doacross_ull_wait(long long unsigned int, ...);
void __builtin_GOMP_doacross_wait(long int, ...);
__edg_bool_type__ __builtin_GOMP_loop_doacross_dynamic_start(unsigned int, long int*, long int, long int*, long int*);
__edg_bool_type__ __builtin_GOMP_loop_doacross_guided_start(unsigned int, long int*, long int, long int*, long int*);
__edg_bool_type__ __builtin_GOMP_loop_doacross_runtime_start(unsigned int, long int*, long int*, long int*);
__edg_bool_type__ __builtin_GOMP_loop_doacross_start(unsigned int, long int*, long int, long int, long int*, long int*, void*, void*);
__edg_bool_type__ __builtin_GOMP_loop_doacross_static_start(unsigned int, long int*, long int, long int*, long int*);
__edg_bool_type__ __builtin_GOMP_loop_dynamic_next(long int*, long int*);
__edg_bool_type__ __builtin_GOMP_loop_dynamic_start(long int, long int, long int, long int, long int*, long int*);
__edg_bool_type__ __builtin_GOMP_loop_guided_next(long int*, long int*);
__edg_bool_type__ __builtin_GOMP_loop_guided_start(long int, long int, long int, long int, long int*, long int*);
__edg_bool_type__ __builtin_GOMP_loop_maybe_nonmonotonic_runtime_next(long int*, long int*);
__edg_bool_type__ __builtin_GOMP_loop_maybe_nonmonotonic_runtime_start(long int, long int, long int, long int*, long int*);
__edg_bool_type__ __builtin_GOMP_loop_nonmonotonic_dynamic_next(long int*, long int*);
__edg_bool_type__ __builtin_GOMP_loop_nonmonotonic_dynamic_start(long int, long int, long int, long int, long int*, long int*);
__edg_bool_type__ __builtin_GOMP_loop_nonmonotonic_guided_next(long int*, long int*);
__edg_bool_type__ __builtin_GOMP_loop_nonmonotonic_guided_start(long int, long int, long int, long int, long int*, long int*);
__edg_bool_type__ __builtin_GOMP_loop_nonmonotonic_runtime_next(long int*, long int*);
__edg_bool_type__ __builtin_GOMP_loop_nonmonotonic_runtime_start(long int, long int, long int, long int*, long int*);
__edg_bool_type__ __builtin_GOMP_loop_ordered_dynamic_next(long int*, long int*);
__edg_bool_type__ __builtin_GOMP_loop_ordered_dynamic_start(long int, long int, long int, long int, long int*, long int*);
__edg_bool_type__ __builtin_GOMP_loop_ordered_guided_next(long int*, long int*);
__edg_bool_type__ __builtin_GOMP_loop_ordered_guided_start(long int, long int, long int, long int, long int*, long int*);
__edg_bool_type__ __builtin_GOMP_loop_ordered_runtime_next(long int*, long int*);
__edg_bool_type__ __builtin_GOMP_loop_ordered_runtime_start(long int, long int, long int, long int*, long int*);
__edg_bool_type__ __builtin_GOMP_loop_ordered_start(long int, long int, long int, long int, long int, long int*, long int*, void*, void*);
__edg_bool_type__ __builtin_GOMP_loop_ordered_static_next(long int*, long int*);
__edg_bool_type__ __builtin_GOMP_loop_ordered_static_start(long int, long int, long int, long int, long int*, long int*);
__edg_bool_type__ __builtin_GOMP_loop_runtime_next(long int*, long int*);
__edg_bool_type__ __builtin_GOMP_loop_runtime_start(long int, long int, long int, long int*, long int*);
__edg_bool_type__ __builtin_GOMP_loop_start(long int, long int, long int, long int, long int, long int*, long int*, void*, void*);
__edg_bool_type__ __builtin_GOMP_loop_static_next(long int*, long int*);
__edg_bool_type__ __builtin_GOMP_loop_static_start(long int, long int, long int, long int, long int*, long int*);
__edg_bool_type__ __builtin_GOMP_loop_ull_doacross_dynamic_start(unsigned int, long long unsigned int*, long long unsigned int, long long unsigned int*, long long unsigned int*);
__edg_bool_type__ __builtin_GOMP_loop_ull_doacross_guided_start(unsigned int, long long unsigned int*, long long unsigned int, long long unsigned int*, long long unsigned int*);
__edg_bool_type__ __builtin_GOMP_loop_ull_doacross_runtime_start(unsigned int, long long unsigned int*, long long unsigned int*, long long unsigned int*);
__edg_bool_type__ __builtin_GOMP_loop_ull_doacross_start(unsigned int, long long unsigned int*, long int, long long unsigned int, long long unsigned int*, long long unsigned int*, void*, void*);
__edg_bool_type__ __builtin_GOMP_loop_ull_doacross_static_start(unsigned int, long long unsigned int*, long long unsigned int, long long unsigned int*, long long unsigned int*);
__edg_bool_type__ __builtin_GOMP_loop_ull_dynamic_next(long long unsigned int*, long long unsigned int*);
__edg_bool_type__ __builtin_GOMP_loop_ull_dynamic_start(__edg_bool_type__, long long unsigned int, long long unsigned int, long long unsigned int, long long unsigned int, long long unsigned int*, long long unsigned int*);
__edg_bool_type__ __builtin_GOMP_loop_ull_guided_next(long long unsigned int*, long long unsigned int*);
__edg_bool_type__ __builtin_GOMP_loop_ull_guided_start(__edg_bool_type__, long long unsigned int, long long unsigned int, long long unsigned int, long long unsigned int, long long unsigned int*, long long unsigned int*);
__edg_bool_type__ __builtin_GOMP_loop_ull_maybe_nonmonotonic_runtime_next(long long unsigned int*, long long unsigned int*);
__edg_bool_type__ __builtin_GOMP_loop_ull_maybe_nonmonotonic_runtime_start(__edg_bool_type__, long long unsigned int, long long unsigned int, long long unsigned int, long long unsigned int*, long long unsigned int*);
__edg_bool_type__ __builtin_GOMP_loop_ull_nonmonotonic_dynamic_next(long long unsigned int*, long long unsigned int*);
__edg_bool_type__ __builtin_GOMP_loop_ull_nonmonotonic_dynamic_start(__edg_bool_type__, long long unsigned int, long long unsigned int, long long unsigned int, long long unsigned int, long long unsigned int*, long long unsigned int*);
__edg_bool_type__ __builtin_GOMP_loop_ull_nonmonotonic_guided_next(long long unsigned int*, long long unsigned int*);
__edg_bool_type__ __builtin_GOMP_loop_ull_nonmonotonic_guided_start(__edg_bool_type__, long long unsigned int, long long unsigned int, long long unsigned int, long long unsigned int, long long unsigned int*, long long unsigned int*);
__edg_bool_type__ __builtin_GOMP_loop_ull_nonmonotonic_runtime_next(long long unsigned int*, long long unsigned int*);
__edg_bool_type__ __builtin_GOMP_loop_ull_nonmonotonic_runtime_start(__edg_bool_type__, long long unsigned int, long long unsigned int, long long unsigned int, long long unsigned int*, long long unsigned int*);
__edg_bool_type__ __builtin_GOMP_loop_ull_ordered_dynamic_next(long long unsigned int*, long long unsigned int*);
__edg_bool_type__ __builtin_GOMP_loop_ull_ordered_dynamic_start(__edg_bool_type__, long long unsigned int, long long unsigned int, long long unsigned int, long long unsigned int, long long unsigned int*, long long unsigned int*);
__edg_bool_type__ __builtin_GOMP_loop_ull_ordered_guided_next(long long unsigned int*, long long unsigned int*);
__edg_bool_type__ __builtin_GOMP_loop_ull_ordered_guided_start(__edg_bool_type__, long long unsigned int, long long unsigned int, long long unsigned int, long long unsigned int, long long unsigned int*, long long unsigned int*);
__edg_bool_type__ __builtin_GOMP_loop_ull_ordered_runtime_next(long long unsigned int*, long long unsigned int*);
__edg_bool_type__ __builtin_GOMP_loop_ull_ordered_runtime_start(__edg_bool_type__, long long unsigned int, long long unsigned int, long long unsigned int, long long unsigned int*, long long unsigned int*);
__edg_bool_type__ __builtin_GOMP_loop_ull_ordered_start(__edg_bool_type__, long long unsigned int, long long unsigned int, long long unsigned int, long int, long long unsigned int, long long unsigned int*, long long unsigned int*, void*, void*);
__edg_bool_type__ __builtin_GOMP_loop_ull_ordered_static_next(long long unsigned int*, long long unsigned int*);
__edg_bool_type__ __builtin_GOMP_loop_ull_ordered_static_start(__edg_bool_type__, long long unsigned int, long long unsigned int, long long unsigned int, long long unsigned int, long long unsigned int*, long long unsigned int*);
__edg_bool_type__ __builtin_GOMP_loop_ull_runtime_next(long long unsigned int*, long long unsigned int*);
__edg_bool_type__ __builtin_GOMP_loop_ull_runtime_start(__edg_bool_type__, long long unsigned int, long long unsigned int, long long unsigned int, long long unsigned int*, long long unsigned int*);
__edg_bool_type__ __builtin_GOMP_loop_ull_start(__edg_bool_type__, long long unsigned int, long long unsigned int, long long unsigned int, long int, long long unsigned int, long long unsigned int*, long long unsigned int*, void*, void*);
__edg_bool_type__ __builtin_GOMP_loop_ull_static_next(long long unsigned int*, long long unsigned int*);
__edg_bool_type__ __builtin_GOMP_loop_ull_static_start(__edg_bool_type__, long long unsigned int, long long unsigned int, long long unsigned int, long long unsigned int, long long unsigned int*, long long unsigned int*);
void __builtin_GOMP_offload_register_ver(int, void*, int, void*);
void __builtin_GOMP_offload_unregister_ver(int, void*, int, void*);
void __builtin_GOMP_parallel(void (*)(void*), void*, unsigned int, unsigned int);
void __builtin_GOMP_parallel_loop_dynamic(void (*)(void*), void*, unsigned int, long int, long int, long int, long int, unsigned int);
void __builtin_GOMP_parallel_loop_guided(void (*)(void*), void*, unsigned int, long int, long int, long int, long int, unsigned int);
void __builtin_GOMP_parallel_loop_maybe_nonmonotonic_runtime(void (*)(void*), void*, unsigned int, long int, long int, long int, unsigned int);
void __builtin_GOMP_parallel_loop_nonmonotonic_dynamic(void (*)(void*), void*, unsigned int, long int, long int, long int, long int, unsigned int);
void __builtin_GOMP_parallel_loop_nonmonotonic_guided(void (*)(void*), void*, unsigned int, long int, long int, long int, long int, unsigned int);
void __builtin_GOMP_parallel_loop_nonmonotonic_runtime(void (*)(void*), void*, unsigned int, long int, long int, long int, unsigned int);
void __builtin_GOMP_parallel_loop_runtime(void (*)(void*), void*, unsigned int, long int, long int, long int, unsigned int);
void __builtin_GOMP_parallel_loop_static(void (*)(void*), void*, unsigned int, long int, long int, long int, long int, unsigned int);
unsigned int __builtin_GOMP_parallel_reductions(void (*)(void*), void*, unsigned int, unsigned int);
void __builtin_GOMP_parallel_sections(void (*)(void*), void*, unsigned int, unsigned int, unsigned int);
unsigned int __builtin_GOMP_sections2_start(unsigned int, void*, void*);
unsigned int __builtin_GOMP_sections_start(unsigned int);
void __builtin_GOMP_single_copy_end(void*);
void __builtin_GOMP_target_data_ext(int, long unsigned int, void*, void*, void*);
void __builtin_GOMP_target_enter_exit_data(int, long unsigned int, void*, void*, void*, unsigned int, void*);
void __builtin_GOMP_target_ext(int, void (*)(void*), long unsigned int, void*, void*, void*, unsigned int, void*, void*);
void __builtin_GOMP_target_update_ext(int, long unsigned int, void*, void*, void*, unsigned int, void*);
#if __COVERITY_GCC_VERSION_AT_LEAST(6, 0)
void __builtin_GOMP_task(void (*)(void*), void*, void (*)(void*, void*), long int, long int, __edg_bool_type__, unsigned int, void*, int);
#endif
void __builtin_GOMP_taskgroup_reduction_register(void*);
void __builtin_GOMP_taskgroup_reduction_unregister(void*);
void __builtin_GOMP_taskloop(void (*)(void*), void*, void (*)(void*, void*), long int, long int, unsigned int, long int, int, long int, long int, long int);
void __builtin_GOMP_taskloop_ull(void (*)(void*), void*, void (*)(void*, void*), long int, long int, unsigned int, long int, int, long long unsigned int, long long unsigned int, long long unsigned int);
void __builtin_GOMP_task_reduction_remap(long unsigned int, long unsigned int, void*);
void __builtin_GOMP_taskwait_depend(void*);
void __builtin_GOMP_teams(unsigned int, unsigned int);
void __builtin_GOMP_teams_reg(void (*)(void*), void*, unsigned int, unsigned int, unsigned int);
void __builtin_GOMP_workshare_task_reduction_unregister(__edg_bool_type__);
void* __builtin_GOMP_alloc(long unsigned int, long unsigned int, long int);
void __builtin_GOMP_free(void*, long int);

#if __COVERITY_GCC_VERSION_AT_LEAST(12, 0)
void __builtin_GOACC_barrier();
void __builtin_GOACC_enter_data(int, long unsigned int, void*, void*, void*, int, int, ...);
void __builtin_GOACC_exit_data(int, long unsigned int, void*, void*, void*, int, int, ...);
void __builtin_GOACC_single_copy_end(void*);
void* __builtin_GOACC_single_copy_start();
__cov_bool __builtin_GOACC_single_start();
void __builtin_GOMP_error(const void*, long unsigned int);
void __builtin_GOMP_scope_start(void*);
__cov_bool __builtin_GOMP_teams4(unsigned int, unsigned int, unsigned int, __cov_bool);
void __builtin_GOMP_warning(const void*, long unsigned int);
#endif /* GCC >= 12.0 */
#endif /* _OPENMP */
/* Copyright (c) 2023 Synopsys, Inc. All rights reserved worldwide. */
#ifdef __coverity_cplusplus
extern "C" {
#endif

#ifdef __PPC__

typedef int __attribute__((vector_size(8))) __ev64_opaque__;
typedef int __ppc_v2i __attribute__ ((__vector_size__ (8)));

int __builtin_spe_brinc(int, int);
__ppc_v2i __builtin_spe_evabs(__ppc_v2i);
__ppc_v2i __builtin_spe_evaddiw(__ppc_v2i, char);
__ppc_v2i __builtin_spe_evaddsmiaaw(__ppc_v2i);
__ppc_v2i __builtin_spe_evaddssiaaw(__ppc_v2i);
__ppc_v2i __builtin_spe_evaddumiaaw(__ppc_v2i);
__ppc_v2i __builtin_spe_evaddusiaaw(__ppc_v2i);
__ppc_v2i __builtin_spe_evaddw(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evand(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evandc(__ppc_v2i, __ppc_v2i);
int __builtin_spe_evcmpeq(int, __ppc_v2i, __ppc_v2i);
int __builtin_spe_evcmpgts(int, __ppc_v2i, __ppc_v2i);
int __builtin_spe_evcmpgtu(int, __ppc_v2i, __ppc_v2i);
int __builtin_spe_evcmplts(int, __ppc_v2i, __ppc_v2i);
int __builtin_spe_evcmpltu(int, __ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evcntlsw(__ppc_v2i);
__ppc_v2i __builtin_spe_evcntlzw(__ppc_v2i);
__ppc_v2i __builtin_spe_evdivws(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evdivwu(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_eveqv(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evextsb(__ppc_v2i);
__ppc_v2i __builtin_spe_evextsh(__ppc_v2i);
__ppc_v2i __builtin_spe_evfsabs(__ppc_v2i);
__ppc_v2i __builtin_spe_evfsadd(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evfscfsf(__ppc_v2i);
__ppc_v2i __builtin_spe_evfscfsi(__ppc_v2i);
__ppc_v2i __builtin_spe_evfscfuf(__ppc_v2i);
__ppc_v2i __builtin_spe_evfscfui(__ppc_v2i);
int __builtin_spe_evfscmpeq(int, __ppc_v2i, __ppc_v2i);
int __builtin_spe_evfscmpgt(int, __ppc_v2i, __ppc_v2i);
int __builtin_spe_evfscmplt(int, __ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evfsctsf(__ppc_v2i);
__ppc_v2i __builtin_spe_evfsctsi(__ppc_v2i);
__ppc_v2i __builtin_spe_evfsctsiz(__ppc_v2i);
__ppc_v2i __builtin_spe_evfsctuf(__ppc_v2i);
__ppc_v2i __builtin_spe_evfsctui(__ppc_v2i);
__ppc_v2i __builtin_spe_evfsctuiz(__ppc_v2i);
__ppc_v2i __builtin_spe_evfsdiv(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evfsmul(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evfsnabs(__ppc_v2i);
__ppc_v2i __builtin_spe_evfsneg(__ppc_v2i);
__ppc_v2i __builtin_spe_evfssub(__ppc_v2i, __ppc_v2i);
int __builtin_spe_evfststeq(int, __ppc_v2i, __ppc_v2i);
int __builtin_spe_evfststgt(int, __ppc_v2i, __ppc_v2i);
int __builtin_spe_evfststlt(int, __ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evldd(__ppc_v2i*, int);
__ppc_v2i __builtin_spe_evlddx(__ppc_v2i*, int);
__ppc_v2i __builtin_spe_evldh(__ppc_v2i*, int);
__ppc_v2i __builtin_spe_evldhx(__ppc_v2i*, int);
__ppc_v2i __builtin_spe_evldw(__ppc_v2i*, int);
__ppc_v2i __builtin_spe_evldwx(__ppc_v2i*, int);
__ppc_v2i __builtin_spe_evlhhesplat(short unsigned int*, int);
__ppc_v2i __builtin_spe_evlhhesplatx(short unsigned int*, int);
__ppc_v2i __builtin_spe_evlhhossplat(short unsigned int*, int);
__ppc_v2i __builtin_spe_evlhhossplatx(short unsigned int*, int);
__ppc_v2i __builtin_spe_evlhhousplat(short unsigned int*, int);
__ppc_v2i __builtin_spe_evlhhousplatx(short unsigned int*, int);
__ppc_v2i __builtin_spe_evlwhe(unsigned int*, int);
__ppc_v2i __builtin_spe_evlwhex(unsigned int*, int);
__ppc_v2i __builtin_spe_evlwhos(unsigned int*, int);
__ppc_v2i __builtin_spe_evlwhosx(unsigned int*, int);
__ppc_v2i __builtin_spe_evlwhou(unsigned int*, int);
__ppc_v2i __builtin_spe_evlwhoux(unsigned int*, int);
__ppc_v2i __builtin_spe_evlwhsplat(unsigned int*, int);
__ppc_v2i __builtin_spe_evlwhsplatx(unsigned int*, int);
__ppc_v2i __builtin_spe_evlwwsplat(unsigned int*, int);
__ppc_v2i __builtin_spe_evlwwsplatx(unsigned int*, int);
__ppc_v2i __builtin_spe_evmergehi(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evmergehilo(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evmergelo(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evmergelohi(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evmhegsmfaa(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evmhegsmfan(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evmhegsmiaa(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evmhegsmian(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evmhegumiaa(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evmhegumian(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evmhesmf(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evmhesmfa(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evmhesmfaaw(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evmhesmfanw(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evmhesmi(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evmhesmia(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evmhesmiaaw(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evmhesmianw(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evmhessf(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evmhessfa(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evmhessfaaw(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evmhessfanw(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evmhessiaaw(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evmhessianw(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evmheumi(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evmheumia(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evmheumiaaw(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evmheumianw(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evmheusiaaw(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evmheusianw(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evmhogsmfaa(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evmhogsmfan(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evmhogsmiaa(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evmhogsmian(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evmhogumiaa(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evmhogumian(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evmhosmf(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evmhosmfa(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evmhosmfaaw(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evmhosmfanw(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evmhosmi(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evmhosmia(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evmhosmiaaw(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evmhosmianw(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evmhossf(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evmhossfa(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evmhossfaaw(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evmhossfanw(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evmhossiaaw(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evmhossianw(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evmhoumi(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evmhoumia(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evmhoumiaaw(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evmhoumianw(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evmhousiaaw(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evmhousianw(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evmra(__ppc_v2i);
__ppc_v2i __builtin_spe_evmwhsmf(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evmwhsmfa(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evmwhsmi(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evmwhsmia(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evmwhssf(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evmwhssfa(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evmwhumi(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evmwhumia(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evmwlsmiaaw(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evmwlsmianw(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evmwlssiaaw(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evmwlssianw(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evmwlumi(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evmwlumia(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evmwlumiaaw(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evmwlumianw(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evmwlusiaaw(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evmwlusianw(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evmwsmf(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evmwsmfa(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evmwsmfaa(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evmwsmfan(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evmwsmi(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evmwsmia(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evmwsmiaa(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evmwsmian(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evmwssf(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evmwssfa(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evmwssfaa(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evmwssfan(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evmwumi(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evmwumia(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evmwumiaa(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evmwumian(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evnand(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evneg(__ppc_v2i);
__ppc_v2i __builtin_spe_evnor(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evor(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evorc(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evrlw(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evrlwi(__ppc_v2i, char);
__ppc_v2i __builtin_spe_evrndw(__ppc_v2i);
__ppc_v2i __builtin_spe_evsel_eq(__ppc_v2i, __ppc_v2i, __ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evsel_fseq(__ppc_v2i, __ppc_v2i, __ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evsel_fsgt(__ppc_v2i, __ppc_v2i, __ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evsel_fslt(__ppc_v2i, __ppc_v2i, __ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evsel_fststeq(__ppc_v2i, __ppc_v2i, __ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evsel_fststgt(__ppc_v2i, __ppc_v2i, __ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evsel_fststlt(__ppc_v2i, __ppc_v2i, __ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evsel_gts(__ppc_v2i, __ppc_v2i, __ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evsel_gtu(__ppc_v2i, __ppc_v2i, __ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evsel_lts(__ppc_v2i, __ppc_v2i, __ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evsel_ltu(__ppc_v2i, __ppc_v2i, __ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evslw(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evslwi(__ppc_v2i, char);
__ppc_v2i __builtin_spe_evsplatfi(signed char);
__ppc_v2i __builtin_spe_evsplati(signed char);
__ppc_v2i __builtin_spe_evsrwis(__ppc_v2i, char);
__ppc_v2i __builtin_spe_evsrwiu(__ppc_v2i, char);
__ppc_v2i __builtin_spe_evsrws(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evsrwu(__ppc_v2i, __ppc_v2i);
void __builtin_spe_evstdd(__ppc_v2i, __ppc_v2i*, char);
void __builtin_spe_evstddx(__ppc_v2i, __ppc_v2i*, int);
void __builtin_spe_evstdh(__ppc_v2i, __ppc_v2i*, char);
void __builtin_spe_evstdhx(__ppc_v2i, __ppc_v2i*, int);
void __builtin_spe_evstdw(__ppc_v2i, __ppc_v2i*, char);
void __builtin_spe_evstdwx(__ppc_v2i, __ppc_v2i*, int);
void __builtin_spe_evstwhe(__ppc_v2i, unsigned int*, char);
void __builtin_spe_evstwhex(__ppc_v2i, unsigned int*, int);
void __builtin_spe_evstwho(__ppc_v2i, unsigned int*, char);
void __builtin_spe_evstwhox(__ppc_v2i, unsigned int*, int);
void __builtin_spe_evstwwe(__ppc_v2i, unsigned int*, char);
void __builtin_spe_evstwwex(__ppc_v2i, unsigned int*, int);
void __builtin_spe_evstwwo(__ppc_v2i, unsigned int*, char);
void __builtin_spe_evstwwox(__ppc_v2i, unsigned int*, int);
__ppc_v2i __builtin_spe_evsubfsmiaaw(__ppc_v2i);
__ppc_v2i __builtin_spe_evsubfssiaaw(__ppc_v2i);
__ppc_v2i __builtin_spe_evsubfumiaaw(__ppc_v2i);
__ppc_v2i __builtin_spe_evsubfusiaaw(__ppc_v2i);
__ppc_v2i __builtin_spe_evsubfw(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe_evsubifw(__ppc_v2i, char);
__ppc_v2i __builtin_spe_evxor(__ppc_v2i, __ppc_v2i);
int __builtin_spe_mfspefscr();
void __builtin_spe_mtspefscr(int);

__ppc_v2i __builtin_spe2_evaddwss(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe2_evaddhss(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe2_evsubfwss(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe2_evsubfhss(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe2_evsatsdsw(__ppc_v2i);
__ppc_v2i __internal_ev_mwhssfanw(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe2_evmwhssfanw(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe2_evmwhssfaaw(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe2_evdotpwcssf(__ppc_v2i, __ppc_v2i);
__ppc_v2i __builtin_spe2_evdotphihcssf(__ppc_v2i, __ppc_v2i);

#endif

#ifdef __coverity_cplusplus
}
#endif

#ifdef __COVERITY_ALTIVEC__
/* Declarations of AltiVec PPU built-in intrinsic functions. */
/* Many of these are overloaded, so they can't be declared in C.  For those
 * that have operator equivalents those equivalents are used in C instead,
 * otherwise a generic version with no parameter types is declared.
 */

typedef vector int __coverity_generic_altivec_vector;

#ifdef __coverity_cplusplus
extern vector signed char __builtin_vec_abs (vector signed char a1);
extern vector signed short __builtin_vec_abs (vector signed short a1);
extern vector signed int __builtin_vec_abs (vector signed int a1);
extern vector float __builtin_vec_abs (vector float a1);

extern vector signed char __builtin_vec_abss (vector signed char a1);
extern vector signed short __builtin_vec_abss (vector signed short a1);
#else
__coverity_generic_altivec_vector  __builtin_vec_abs();
__coverity_generic_altivec_vector  __builtin_vec_abss();
#endif

#ifdef __coverity_cplusplus
    extern vector unsigned int __builtin_vec_vslw (vector unsigned int a1, vector unsigned int a2);
#else
__coverity_generic_altivec_vector  __builtin_vec_vslw ();
#endif

#ifdef __coverity_cplusplus
extern int __builtin_vec_vcmpgt_p(int, vector signed char a1, vector signed char a2);
extern int __builtin_vec_vcmpgt_p(int, vector unsigned char a1, vector unsigned char a2);
extern int __builtin_vec_vcmpgt_p(int, vector signed short a1, vector signed short a2);
extern int __builtin_vec_vcmpgt_p(int, vector unsigned short a1, vector unsigned short a2);
extern int __builtin_vec_vcmpgt_p(int, vector signed int a1, vector signed int a2);
extern int __builtin_vec_vcmpgt_p(int, vector unsigned int a1, vector unsigned int a2);
extern int __builtin_vec_vcmpgt_p(int, vector signed char a1, vector signed char a2);
extern int __builtin_vec_vcmpgt_p(int, vector float a1, vector float a2);
#else
int __builtin_vec_vcmpgt_p();
#endif

#ifdef __coverity_cplusplus
extern vector signed char __builtin_vec_add (vector signed char a1, vector signed char a2);
extern vector unsigned char __builtin_vec_add (vector signed char a1, vector unsigned char a2);

extern vector unsigned char __builtin_vec_add (vector unsigned char a1, vector signed char a2);

extern vector unsigned char __builtin_vec_add (vector unsigned char a1,
                              vector unsigned char a2);
extern vector signed short __builtin_vec_add (vector signed short a1, vector signed short a2);
extern vector unsigned short __builtin_vec_add (vector signed short a1,
                               vector unsigned short a2);
extern vector unsigned short __builtin_vec_add (vector unsigned short a1,
                               vector signed short a2);
extern vector unsigned short __builtin_vec_add (vector unsigned short a1,
                               vector unsigned short a2);
extern vector signed int __builtin_vec_add (vector signed int a1, vector signed int a2);
extern vector unsigned int __builtin_vec_add (vector signed int a1, vector unsigned int a2);
extern vector unsigned int __builtin_vec_add (vector unsigned int a1, vector signed int a2);
extern vector unsigned int __builtin_vec_add (vector unsigned int a1, vector unsigned int a2);
extern vector float __builtin_vec_add (vector float a1, vector float a2);
#else
# define __builtin_vec_add(a, b) ((a) + (b))
#endif

extern vector unsigned int __builtin_vec_addc (vector unsigned int a1, vector unsigned int a2);

#ifdef __coverity_cplusplus
extern vector unsigned char __builtin_vec_adds (vector signed char a1,
                               vector unsigned char a2);
extern vector unsigned char __builtin_vec_adds (vector unsigned char a1,
                               vector signed char a2);
extern vector unsigned char __builtin_vec_adds (vector unsigned char a1,
                               vector unsigned char a2);
extern vector signed char __builtin_vec_adds (vector signed char a1, vector signed char a2);
extern vector unsigned short __builtin_vec_adds (vector signed short a1,
                                vector unsigned short a2);
extern vector unsigned short __builtin_vec_adds (vector unsigned short a1,
                                vector signed short a2);
extern vector unsigned short __builtin_vec_adds (vector unsigned short a1,
                                vector unsigned short a2);
extern vector signed short __builtin_vec_adds (vector signed short a1, vector signed short a2);

extern vector unsigned int __builtin_vec_adds (vector signed int a1, vector unsigned int a2);
extern vector unsigned int __builtin_vec_adds (vector unsigned int a1, vector signed int a2);
extern vector unsigned int __builtin_vec_adds (vector unsigned int a1, vector unsigned int a2);

extern vector signed int __builtin_vec_adds (vector signed int a1, vector signed int a2);
#else
__coverity_generic_altivec_vector  __builtin_vec_adds ();
#endif

#ifdef __coverity_cplusplus
extern vector float __builtin_vec_and (vector float a1, vector float a2);
extern vector float __builtin_vec_and (vector float a1, vector signed int a2);
extern vector float __builtin_vec_and (vector signed int a1, vector float a2);
extern vector signed int __builtin_vec_and (vector signed int a1, vector signed int a2);
extern vector unsigned int __builtin_vec_and (vector signed int a1, vector unsigned int a2);
extern vector unsigned int __builtin_vec_and (vector unsigned int a1, vector signed int a2);
extern vector unsigned int __builtin_vec_and (vector unsigned int a1, vector unsigned int a2);
extern vector signed short __builtin_vec_and (vector signed short a1, vector signed short a2);
extern vector unsigned short __builtin_vec_and (vector signed short a1,
                               vector unsigned short a2);
extern vector unsigned short __builtin_vec_and (vector unsigned short a1,
                               vector signed short a2);
extern vector unsigned short __builtin_vec_and (vector unsigned short a1,
                               vector unsigned short a2);
extern  vector signed char __builtin_vec_and (vector signed char a1, vector signed char a2);
extern vector unsigned char __builtin_vec_and (vector signed char a1, vector unsigned char a2);

extern vector unsigned char __builtin_vec_and (vector unsigned char a1, vector signed char a2);

extern vector unsigned char __builtin_vec_and (vector unsigned char a1,
                              vector unsigned char a2);
#else
# define __builtin_vec_and(a, b) ((a) & (b))
#endif

#ifdef __coverity_cplusplus
extern vector float __builtin_vec_andc (vector float a1, vector float a2);
extern vector float __builtin_vec_andc (vector float a1, vector signed int a2);
extern vector float __builtin_vec_andc (vector signed int a1, vector float a2);
extern vector signed int __builtin_vec_andc (vector signed int a1, vector signed int a2);
extern vector unsigned int __builtin_vec_andc (vector signed int a1, vector unsigned int a2);
extern vector unsigned int __builtin_vec_andc (vector unsigned int a1, vector signed int a2);
extern vector unsigned int __builtin_vec_andc (vector unsigned int a1, vector unsigned int a2);

extern vector signed short __builtin_vec_andc (vector signed short a1, vector signed short a2);

extern vector unsigned short __builtin_vec_andc (vector signed short a1,
                                vector unsigned short a2);
extern vector unsigned short __builtin_vec_andc (vector unsigned short a1,
                                vector signed short a2);
extern vector unsigned short __builtin_vec_andc (vector unsigned short a1,
                                vector unsigned short a2);
extern vector signed char __builtin_vec_andc (vector signed char a1, vector signed char a2);
extern vector unsigned char __builtin_vec_andc (vector signed char a1,
                               vector unsigned char a2);
extern vector unsigned char __builtin_vec_andc (vector unsigned char a1,
                               vector signed char a2);
extern vector unsigned char __builtin_vec_andc (vector unsigned char a1,
                               vector unsigned char a2);
#else
__coverity_generic_altivec_vector  __builtin_vec_andc ();
#endif

#ifdef __coverity_cplusplus
extern vector unsigned char __builtin_vec_avg (vector unsigned char a1,
                              vector unsigned char a2);
extern vector signed char __builtin_vec_avg (vector signed char a1, vector signed char a2);
extern vector unsigned short __builtin_vec_avg (vector unsigned short a1,
                               vector unsigned short a2);
extern vector signed short __builtin_vec_avg (vector signed short a1, vector signed short a2);
extern vector unsigned int __builtin_vec_avg (vector unsigned int a1, vector unsigned int a2);
extern vector signed int __builtin_vec_avg (vector signed int a1, vector signed int a2);
#else
__coverity_generic_altivec_vector  __builtin_vec_avg ();
#endif

extern vector float __builtin_vec_ceil (vector float a1);

extern vector signed int __builtin_vec_cmpb (vector float a1, vector float a2);

#ifdef __coverity_cplusplus
extern vector signed char __builtin_vec_cmpeq (vector signed char a1, vector signed char a2);
extern vector signed char __builtin_vec_cmpeq (vector unsigned char a1,
                              vector unsigned char a2);
extern vector signed short __builtin_vec_cmpeq (vector signed short a1,
                               vector signed short a2);
extern vector signed short __builtin_vec_cmpeq (vector unsigned short a1,
                               vector unsigned short a2);
extern vector signed int __builtin_vec_cmpeq (vector signed int a1, vector signed int a2);
extern vector signed int __builtin_vec_cmpeq (vector unsigned int a1, vector unsigned int a2);
extern vector signed int __builtin_vec_cmpeq (vector float a1, vector float a2);

extern vector signed int __builtin_vec_cmpge (vector float a1, vector float a2);

extern vector signed char __builtin_vec_cmpgt (vector unsigned char a1,
                              vector unsigned char a2);
extern vector signed char __builtin_vec_cmpgt (vector signed char a1, vector signed char a2);
extern vector signed short __builtin_vec_cmpgt (vector unsigned short a1,
                               vector unsigned short a2);
extern vector signed short __builtin_vec_cmpgt (vector signed short a1,
                               vector signed short a2);
extern vector signed int __builtin_vec_cmpgt (vector unsigned int a1, vector unsigned int a2);
extern vector signed int __builtin_vec_cmpgt (vector signed int a1, vector signed int a2);
extern vector signed int __builtin_vec_cmpgt (vector float a1, vector float a2);

extern vector signed int __builtin_vec_cmple (vector float a1, vector float a2);

extern vector signed char __builtin_vec_cmplt (vector unsigned char a1,
                              vector unsigned char a2);
extern vector signed char __builtin_vec_cmplt (vector signed char a1, vector signed char a2);
extern vector signed short __builtin_vec_cmplt (vector unsigned short a1,
                               vector unsigned short a2);
extern vector signed short __builtin_vec_cmplt (vector signed short a1,
                               vector signed short a2);
extern vector signed int __builtin_vec_cmplt (vector unsigned int a1, vector unsigned int a2);
extern vector signed int __builtin_vec_cmplt (vector signed int a1, vector signed int a2);
extern vector signed int __builtin_vec_cmplt (vector float a1, vector float a2);
#else
__coverity_generic_altivec_vector  __builtin_vec_cmpeq ();
__coverity_generic_altivec_vector  __builtin_vec_cmpge ();
__coverity_generic_altivec_vector  __builtin_vec_cmpgt ();
__coverity_generic_altivec_vector  __builtin_vec_cmple ();
__coverity_generic_altivec_vector  __builtin_vec_cmplt ();
#endif

#ifdef __coverity_cplusplus
extern vector float __builtin_vec_ctf (vector unsigned int a1, const char a2);
extern vector float __builtin_vec_ctf (vector signed int a1, const char a2);
#else
extern vector float __builtin_vec_ctf ();
#endif

extern vector signed int __builtin_vec_cts (vector float a1, const char a2);

extern vector unsigned int __builtin_vec_ctu (vector float a1, const char a2);

extern void __builtin_vec_dss (const char a1);

extern void __builtin_vec_dssall (void);

extern void __builtin_vec_dst (const void * a1, int a2, const char a3);

extern void __builtin_vec_dstst (const void * a1, int a2, const char a3);

extern void __builtin_vec_dststt (const void * a1, int a2, const char a3);

extern void __builtin_vec_dstt (const void * a1, int a2, const char a3);

#ifdef __coverity_cplusplus
extern vector float __builtin_vec_expte (vector float a1, vector float a2);
extern vector float __builtin_vec_expte (vector float a1);

extern vector float __builtin_vec_floor (vector float a1, vector float a2);
extern vector float __builtin_vec_floor (vector float a1);
#else
__coverity_generic_altivec_vector __builtin_vec_expte();
__coverity_generic_altivec_vector __builtin_vec_floor();
#endif

#ifdef __coverity_cplusplus
extern vector float __builtin_vec_ld (int a1, vector float * a2);
extern vector float __builtin_vec_ld (int a1, float * a2);
extern vector signed int __builtin_vec_ld (int a1, int * a2);
extern vector signed int __builtin_vec_ld (int a1, vector signed int * a2);
extern vector unsigned int __builtin_vec_ld (int a1, vector unsigned int * a2);
extern vector unsigned int __builtin_vec_ld (int a1, unsigned int * a2);
extern vector signed short __builtin_vec_ld (int a1, short * a2, vector signed short * a3);
extern vector unsigned short __builtin_vec_ld (int a1, unsigned short * a2,
                              vector unsigned short * a3);
extern vector signed char __builtin_vec_ld (int a1, signed char * a2);
extern vector signed char __builtin_vec_ld (int a1, vector signed char * a2);
extern vector unsigned char __builtin_vec_ld (int a1, unsigned char * a2);
extern vector unsigned char __builtin_vec_ld (int a1, vector unsigned char * a2);

extern vector signed char __builtin_vec_lde (int a1, signed char * a2);
extern vector unsigned char __builtin_vec_lde (int a1, unsigned char * a2);
extern vector signed short __builtin_vec_lde (int a1, short * a2);
extern vector unsigned short __builtin_vec_lde (int a1, unsigned short * a2);
extern vector float __builtin_vec_lde (int a1, float * a2);
extern vector signed int __builtin_vec_lde (int a1, int * a2);
extern vector unsigned int __builtin_vec_lde (int a1, unsigned int * a2);

extern vector float __builtin_vec_ldl (int a1, float * a2);
extern vector float __builtin_vec_ldl (int a1, vector float * a2);
extern vector signed int __builtin_vec_ldl (int a1, vector signed int * a2);
extern vector signed int __builtin_vec_ldl (int a1, int * a2);
extern vector unsigned int __builtin_vec_ldl (int a1, unsigned int * a2);
extern vector unsigned int __builtin_vec_ldl (int a1, vector unsigned int * a2);
extern vector signed short __builtin_vec_ldl (int a1, vector signed short * a2);
extern vector signed short __builtin_vec_ldl (int a1, short * a2);
extern vector unsigned short __builtin_vec_ldl (int a1, vector unsigned short * a2);
extern vector unsigned short __builtin_vec_ldl (int a1, unsigned short * a2);
extern vector signed char __builtin_vec_ldl (int a1, vector signed char * a2);
extern vector signed char __builtin_vec_ldl (int a1, signed char * a2);
extern vector unsigned char __builtin_vec_ldl (int a1, vector unsigned char * a2);
extern vector unsigned char __builtin_vec_ldl (int a1, unsigned char * a2);
#else
__coverity_generic_altivec_vector  __builtin_vec_ld ();
__coverity_generic_altivec_vector  __builtin_vec_lde ();
__coverity_generic_altivec_vector  __builtin_vec_ldl ();
#endif

extern vector float __builtin_vec_loge (vector float a1);

extern vector unsigned char __builtin_vec_lvsl (int a1, const void * a2, int * a3);

extern vector unsigned char __builtin_vec_lvsr (int a1, const void * a2, int * a3);

extern vector float __builtin_vec_madd (vector float a1, vector float a2, vector float a3);

extern vector signed short __builtin_vec_madds (vector signed short a1, vector signed short a2,
                               vector signed short a3);

#ifdef __coverity_cplusplus
extern vector unsigned char __builtin_vec_max (vector signed char a1, vector unsigned char a2);

extern vector unsigned char __builtin_vec_max (vector unsigned char a1, vector signed char a2);

extern vector unsigned char __builtin_vec_max (vector unsigned char a1,
                              vector unsigned char a2);
extern vector signed char __builtin_vec_max (vector signed char a1, vector signed char a2);
extern vector unsigned short __builtin_vec_max (vector signed short a1,
                               vector unsigned short a2);
extern vector unsigned short __builtin_vec_max (vector unsigned short a1,
                               vector signed short a2);
extern vector unsigned short __builtin_vec_max (vector unsigned short a1,
                               vector unsigned short a2);
extern vector signed short __builtin_vec_max (vector signed short a1, vector signed short a2);
extern vector unsigned int __builtin_vec_max (vector signed int a1, vector unsigned int a2);
extern vector unsigned int __builtin_vec_max (vector unsigned int a1, vector signed int a2);
extern vector unsigned int __builtin_vec_max (vector unsigned int a1, vector unsigned int a2);
extern vector signed int __builtin_vec_max (vector signed int a1, vector signed int a2);
extern vector float __builtin_vec_max (vector float a1, vector float a2);

extern vector signed char __builtin_vec_mergeh (vector signed char a1, vector signed char a2);
extern vector unsigned char __builtin_vec_mergeh (vector unsigned char a1,
                                 vector unsigned char a2);
extern vector signed short __builtin_vec_mergeh (vector signed short a1,
                                vector signed short a2);
extern vector unsigned short __builtin_vec_mergeh (vector unsigned short a1,
                                  vector unsigned short a2);
extern vector float __builtin_vec_mergeh (vector float a1, vector float a2);
extern vector signed int __builtin_vec_mergeh (vector signed int a1, vector signed int a2);
extern vector unsigned int __builtin_vec_mergeh (vector unsigned int a1,
                                vector unsigned int a2);

extern vector signed char __builtin_vec_mergel (vector signed char a1, vector signed char a2);
extern vector unsigned char __builtin_vec_mergel (vector unsigned char a1,
                                 vector unsigned char a2);
extern vector signed short __builtin_vec_mergel (vector signed short a1,
                                vector signed short a2);
extern vector unsigned short __builtin_vec_mergel (vector unsigned short a1,
                                  vector unsigned short a2);
extern vector float __builtin_vec_mergel (vector float a1, vector float a2);
extern vector signed int __builtin_vec_mergel (vector signed int a1, vector signed int a2);
extern vector unsigned int __builtin_vec_mergel (vector unsigned int a1,
                                vector unsigned int a2);
#else
__coverity_generic_altivec_vector  __builtin_vec_max ();
__coverity_generic_altivec_vector  __builtin_vec_mergeh ();
__coverity_generic_altivec_vector  __builtin_vec_mergel ();
#endif

extern vector unsigned short __builtin_vec_mfvscr (void);

#ifdef __coverity_cplusplus
extern vector unsigned char __builtin_vec_min (vector signed char a1, vector unsigned char a2);

extern vector unsigned char __builtin_vec_min (vector unsigned char a1, vector signed char a2);

extern vector unsigned char __builtin_vec_min (vector unsigned char a1,
                              vector unsigned char a2);
extern vector signed char __builtin_vec_min (vector signed char a1, vector signed char a2);
extern vector unsigned short __builtin_vec_min (vector signed short a1,
                               vector unsigned short a2);
extern vector unsigned short __builtin_vec_min (vector unsigned short a1,
                               vector signed short a2);
extern vector unsigned short __builtin_vec_min (vector unsigned short a1,
                               vector unsigned short a2);
extern vector signed short __builtin_vec_min (vector signed short a1, vector signed short a2);
extern vector unsigned int __builtin_vec_min (vector signed int a1, vector unsigned int a2);
extern vector unsigned int __builtin_vec_min (vector unsigned int a1, vector signed int a2);
extern vector unsigned int __builtin_vec_min (vector unsigned int a1, vector unsigned int a2);
extern vector signed int __builtin_vec_min (vector signed int a1, vector signed int a2);
extern vector float __builtin_vec_min (vector float a1, vector float a2);

extern vector signed short __builtin_vec_mladd (vector signed short a1, vector signed short a2,
                               vector signed short a3);
extern vector signed short __builtin_vec_mladd (vector signed short a1,
                               vector unsigned short a2,
                               vector unsigned short a3);
extern vector signed short __builtin_vec_mladd (vector unsigned short a1,
                               vector signed short a2,
                               vector signed short a3);
extern vector unsigned short __builtin_vec_mladd (vector unsigned short a1,
                                 vector unsigned short a2,
                                 vector unsigned short a3);
#else
__coverity_generic_altivec_vector __builtin_vec_min ();
__coverity_generic_altivec_vector __builtin_vec_mladd ();
#endif

extern vector signed short __builtin_vec_mradds (vector signed short a1,
                                vector signed short a2,
                                vector signed short a3);

#ifdef __coverity_cplusplus
extern vector unsigned int __builtin_vec_msum (vector unsigned char a1,
                              vector unsigned char a2,
                              vector unsigned int a3);
extern vector signed int __builtin_vec_msum (vector signed char a1, vector unsigned char a2,
                            vector signed int a3);
extern vector unsigned int __builtin_vec_msum (vector unsigned short a1,
                              vector unsigned short a2,
                              vector unsigned int a3);
extern vector signed int __builtin_vec_msum (vector signed short a1, vector signed short a2,
                            vector signed int a3);

extern vector unsigned int __builtin_vec_msums (vector unsigned short a1,
                               vector unsigned short a2,
                               vector unsigned int a3);
extern vector signed int __builtin_vec_msums (vector signed short a1, vector signed short a2,
                             vector signed int a3);

extern void __builtin_vec_mtvscr (vector signed int a1);
extern void __builtin_vec_mtvscr (vector unsigned int a1);
extern void __builtin_vec_mtvscr (vector signed short a1);
extern void __builtin_vec_mtvscr (vector unsigned short a1);
extern void __builtin_vec_mtvscr (vector signed char a1);
extern void __builtin_vec_mtvscr (vector unsigned char a1);

extern vector unsigned short __builtin_vec_mule (vector unsigned char a1,
                                vector unsigned char a2);
extern vector signed short __builtin_vec_mule (vector signed char a1, vector signed char a2);
extern vector unsigned int __builtin_vec_mule (vector unsigned short a1,
                              vector unsigned short a2);
extern vector signed int __builtin_vec_mule (vector signed short a1, vector signed short a2);

extern vector unsigned short __builtin_vec_mulo (vector unsigned char a1,
                                vector unsigned char a2);
extern vector signed short __builtin_vec_mulo (vector signed char a1, vector signed char a2);
extern vector unsigned int __builtin_vec_mulo (vector unsigned short a1,
                              vector unsigned short a2);
extern vector signed int __builtin_vec_mulo (vector signed short a1, vector signed short a2);
#else
__coverity_generic_altivec_vector __builtin_vec_msum ();
__coverity_generic_altivec_vector __builtin_vec_msums ();
extern void __builtin_vec_mtvscr ();
__coverity_generic_altivec_vector __builtin_vec_mule ();
__coverity_generic_altivec_vector __builtin_vec_mulo ();
#endif

extern vector float __builtin_vec_nmsub (vector float a1, vector float a2, vector float a3);

#ifdef __coverity_cplusplus
extern vector float __builtin_vec_nor (vector float a1, vector float a2);
extern vector signed int __builtin_vec_nor (vector signed int a1, vector signed int a2);
extern vector unsigned int __builtin_vec_nor (vector unsigned int a1, vector unsigned int a2);
extern vector signed short __builtin_vec_nor (vector signed short a1, vector signed short a2);
extern vector unsigned short __builtin_vec_nor (vector unsigned short a1,
                               vector unsigned short a2);
extern vector signed char __builtin_vec_nor (vector signed char a1, vector signed char a2);
extern vector unsigned char __builtin_vec_nor (vector unsigned char a1,
                              vector unsigned char a2);

extern vector float __builtin_vec_or (vector float a1, vector float a2);
extern vector float __builtin_vec_or (vector float a1, vector signed int a2);
extern vector float __builtin_vec_or (vector signed int a1, vector float a2);
extern vector signed int __builtin_vec_or (vector signed int a1, vector signed int a2);
extern vector unsigned int __builtin_vec_or (vector signed int a1, vector unsigned int a2);
extern vector unsigned int __builtin_vec_or (vector unsigned int a1, vector signed int a2);
extern vector unsigned int __builtin_vec_or (vector unsigned int a1, vector unsigned int a2);
extern vector signed short __builtin_vec_or (vector signed short a1, vector signed short a2);
extern vector unsigned short __builtin_vec_or (vector signed short a1,
                              vector unsigned short a2);
extern vector unsigned short __builtin_vec_or (vector unsigned short a1,
                              vector signed short a2);
extern vector unsigned short __builtin_vec_or (vector unsigned short a1,
                              vector unsigned short a2);
extern vector signed char __builtin_vec_or (vector signed char a1, vector signed char a2);
extern vector unsigned char __builtin_vec_or (vector signed char a1, vector unsigned char a2);
extern vector unsigned char __builtin_vec_or (vector unsigned char a1, vector signed char a2);
extern vector unsigned char __builtin_vec_or (vector unsigned char a1,
                             vector unsigned char a2);

extern vector signed char __builtin_vec_pack (vector signed short a1, vector signed short a2);
extern vector unsigned char __builtin_vec_pack (vector unsigned short a1,
                               vector unsigned short a2);
extern vector signed short __builtin_vec_pack (vector signed int a1, vector signed int a2);
extern vector unsigned short __builtin_vec_pack (vector unsigned int a1,
                                vector unsigned int a2);
#else
# define __builtin_vec_nor(a, b) (~((a) | (b)))
# define __builtin_vec_or(a, b) ((a) | (b))
__coverity_generic_altivec_vector __builtin_vec_pack ();
#endif

extern vector signed short __builtin_vec_packpx (vector unsigned int a1,
                                vector unsigned int a2);

#ifdef __coverity_cplusplus
extern vector unsigned char __builtin_vec_packs (vector unsigned short a1,
                                vector unsigned short a2);
extern vector signed char __builtin_vec_packs (vector signed short a1, vector signed short a2);

extern vector unsigned short __builtin_vec_packs (vector unsigned int a1,
                                 vector unsigned int a2);
extern vector signed short __builtin_vec_packs (vector signed int a1, vector signed int a2);

extern vector unsigned char __builtin_vec_packsu (vector unsigned short a1,
                                 vector unsigned short a2);
extern vector unsigned char __builtin_vec_packsu (vector signed short a1,
                                 vector signed short a2);
extern vector unsigned short __builtin_vec_packsu (vector unsigned int a1,
                                  vector unsigned int a2);
extern vector unsigned short __builtin_vec_packsu (vector signed int a1, vector signed int a2);

extern vector float __builtin_vec_perm (vector float a1, vector float a2,
                       vector unsigned char a3);
extern vector signed int __builtin_vec_perm (vector signed int a1, vector signed int a2,
                            vector unsigned char a3);
extern vector unsigned int __builtin_vec_perm (vector unsigned int a1, vector unsigned int a2,
                              vector unsigned char a3);
extern vector signed short __builtin_vec_perm (vector signed short a1, vector signed short a2,
                              vector unsigned char a3);
extern vector unsigned short __builtin_vec_perm (vector unsigned short a1,
                                vector unsigned short a2,
                                vector unsigned char a3);
extern vector signed char __builtin_vec_perm (vector signed char a1, vector signed char a2,
                             vector unsigned char a3);
extern vector unsigned char __builtin_vec_perm (vector unsigned char a1,
                               vector unsigned char a2,
                               vector unsigned char a3);
#else
__coverity_generic_altivec_vector __builtin_vec_packs ();
__coverity_generic_altivec_vector __builtin_vec_packsu ();
__coverity_generic_altivec_vector __builtin_vec_perm ();
#endif

extern vector float __builtin_vec_re (vector float a1);

#ifdef __coverity_cplusplus
extern vector signed char __builtin_vec_rl (vector signed char a1, vector unsigned char a2);
extern vector unsigned char __builtin_vec_rl (vector unsigned char a1,
                             vector unsigned char a2);
extern vector signed short __builtin_vec_rl (vector signed short a1, vector unsigned short a2);

extern vector unsigned short __builtin_vec_rl (vector unsigned short a1,
                              vector unsigned short a2);
extern vector signed int __builtin_vec_rl (vector signed int a1, vector unsigned int a2);
extern vector unsigned int __builtin_vec_rl (vector unsigned int a1, vector unsigned int a2);
#else
__coverity_generic_altivec_vector __builtin_vec_rl ();
#endif

extern vector float __builtin_vec_round (vector float a1);

extern vector float __builtin_vec_rsqrte (vector float a1);

#ifdef __coverity_cplusplus
extern vector float __builtin_vec_sel (vector float a1, vector float a2, vector signed int a3);
extern vector float __builtin_vec_sel (vector float a1, vector float a2, vector unsigned int a3);
extern vector signed int __builtin_vec_sel (vector signed int a1, vector signed int a2,
                           vector signed int a3);
extern vector signed int __builtin_vec_sel (vector signed int a1, vector signed int a2,
                           vector unsigned int a3);
extern vector unsigned int __builtin_vec_sel (vector unsigned int a1, vector unsigned int a2,
                             vector signed int a3);
extern vector unsigned int __builtin_vec_sel (vector unsigned int a1, vector unsigned int a2,
                             vector unsigned int a3);
extern vector signed short __builtin_vec_sel (vector signed short a1, vector signed short a2,
                             vector signed short a3);
extern vector signed short __builtin_vec_sel (vector signed short a1, vector signed short a2,
                             vector unsigned short a3);
extern vector unsigned short __builtin_vec_sel (vector unsigned short a1,
                               vector unsigned short a2,
                               vector signed short a3);
extern vector unsigned short __builtin_vec_sel (vector unsigned short a1,
                               vector unsigned short a2,
                               vector unsigned short a3);
extern vector signed char __builtin_vec_sel (vector signed char a1, vector signed char a2,
                            vector signed char a3);
extern vector signed char __builtin_vec_sel (vector signed char a1, vector signed char a2,
                            vector unsigned char a3);
extern vector unsigned char __builtin_vec_sel (vector unsigned char a1,
                              vector unsigned char a2,
                              vector signed char a3);
extern vector unsigned char __builtin_vec_sel (vector unsigned char a1,
                              vector unsigned char a2,
                              vector unsigned char a3);

extern vector signed char __builtin_vec_sl (vector signed char a1, vector unsigned char a2);
extern vector unsigned char __builtin_vec_sl (vector unsigned char a1,
                             vector unsigned char a2);
extern vector signed short __builtin_vec_sl (vector signed short a1, vector unsigned short a2);

extern vector unsigned short __builtin_vec_sl (vector unsigned short a1,
                              vector unsigned short a2);
extern vector signed int __builtin_vec_sl (vector signed int a1, vector unsigned int a2);
extern vector unsigned int __builtin_vec_sl (vector unsigned int a1, vector unsigned int a2);

extern vector float __builtin_vec_sld (vector float a1, vector float a2, const char a3);
extern vector signed int __builtin_vec_sld (vector signed int a1, vector signed int a2,
                           const char a3);
extern vector unsigned int __builtin_vec_sld (vector unsigned int a1, vector unsigned int a2,
                             const char a3);
extern vector signed short __builtin_vec_sld (vector signed short a1, vector signed short a2,
                             const char a3);
extern vector unsigned short __builtin_vec_sld (vector unsigned short a1,
                               vector unsigned short a2, const char a3);
extern vector signed char __builtin_vec_sld (vector signed char a1, vector signed char a2,
                            const char a3);
extern vector unsigned char __builtin_vec_sld (vector unsigned char a1,
                              vector unsigned char a2,
                              const char a3);

extern vector signed int __builtin_vec_sll (vector signed int a1, vector unsigned int a2);
extern vector signed int __builtin_vec_sll (vector signed int a1, vector unsigned short a2);
extern vector signed int __builtin_vec_sll (vector signed int a1, vector unsigned char a2);
extern vector unsigned int __builtin_vec_sll (vector unsigned int a1, vector unsigned int a2);
extern vector unsigned int __builtin_vec_sll (vector unsigned int a1,
                             vector unsigned short a2);
extern vector unsigned int __builtin_vec_sll (vector unsigned int a1, vector unsigned char a2);

extern vector signed short __builtin_vec_sll (vector signed short a1, vector unsigned int a2);
extern vector signed short __builtin_vec_sll (vector signed short a1,
                             vector unsigned short a2);
extern vector signed short __builtin_vec_sll (vector signed short a1, vector unsigned char a2);

extern vector unsigned short __builtin_vec_sll (vector unsigned short a1,
                               vector unsigned int a2);
extern vector unsigned short __builtin_vec_sll (vector unsigned short a1,
                               vector unsigned short a2);
extern vector unsigned short __builtin_vec_sll (vector unsigned short a1,
                               vector unsigned char a2);
extern vector signed char __builtin_vec_sll (vector signed char a1, vector unsigned int a2);
extern vector signed char __builtin_vec_sll (vector signed char a1, vector unsigned short a2);
extern vector signed char __builtin_vec_sll (vector signed char a1, vector unsigned char a2);
extern vector unsigned char __builtin_vec_sll (vector unsigned char a1,
                              vector unsigned int a2);
extern vector unsigned char __builtin_vec_sll (vector unsigned char a1,
                              vector unsigned short a2);
extern vector unsigned char __builtin_vec_sll (vector unsigned char a1,
                              vector unsigned char a2);

extern vector float __builtin_vec_slo (vector float a1, vector signed char a2);
extern vector float __builtin_vec_slo (vector float a1, vector unsigned char a2);
extern vector signed int __builtin_vec_slo (vector signed int a1, vector signed char a2);
extern vector signed int __builtin_vec_slo (vector signed int a1, vector unsigned char a2);
extern vector unsigned int __builtin_vec_slo (vector unsigned int a1, vector signed char a2);
extern vector unsigned int __builtin_vec_slo (vector unsigned int a1, vector unsigned char a2);

extern vector signed short __builtin_vec_slo (vector signed short a1, vector signed char a2);
extern vector signed short __builtin_vec_slo (vector signed short a1, vector unsigned char a2);

extern vector unsigned short __builtin_vec_slo (vector unsigned short a1,
                               vector signed char a2);
extern vector unsigned short __builtin_vec_slo (vector unsigned short a1,
                               vector unsigned char a2);
extern vector signed char __builtin_vec_slo (vector signed char a1, vector signed char a2);
extern vector signed char __builtin_vec_slo (vector signed char a1, vector unsigned char a2);
extern vector unsigned char __builtin_vec_slo (vector unsigned char a1, vector signed char a2);

extern vector unsigned char __builtin_vec_slo (vector unsigned char a1,
                              vector unsigned char a2);

extern vector signed char __builtin_vec_splat (vector signed char a1, const char a2);
extern vector unsigned char __builtin_vec_splat (vector unsigned char a1, const char a2);
extern vector signed short __builtin_vec_splat (vector signed short a1, const char a2);
extern vector unsigned short __builtin_vec_splat (vector unsigned short a1, const char a2);
extern vector float __builtin_vec_splat (vector float a1, const char a2);
extern vector signed int __builtin_vec_splat (vector signed int a1, const char a2);
extern vector unsigned int __builtin_vec_splat (vector unsigned int a1, const char a2);
#else
# define __builtin_vec_sl(a, b) ((a) << (b))
__coverity_generic_altivec_vector __builtin_vec_sel ();
__coverity_generic_altivec_vector __builtin_vec_sel ();
__coverity_generic_altivec_vector __builtin_vec_sld ();
__coverity_generic_altivec_vector __builtin_vec_sll ();
__coverity_generic_altivec_vector __builtin_vec_slo ();
#endif

extern vector signed char __builtin_vec_splat_s8 (const char a1);

extern vector signed short __builtin_vec_splat_s16 (const char a1);

extern vector signed int __builtin_vec_splat_s32 (const char a1);

extern vector unsigned char __builtin_vec_splat_u8 (const char a1);

extern vector unsigned short __builtin_vec_splat_u16 (const char a1);

extern vector unsigned int __builtin_vec_splat_u32 (const char a1);

#ifdef __coverity_cplusplus
extern vector signed char __builtin_vec_sr (vector signed char a1, vector unsigned char a2);
extern vector unsigned char __builtin_vec_sr (vector unsigned char a1,
                             vector unsigned char a2);
extern vector signed short __builtin_vec_sr (vector signed short a1, vector unsigned short a2);

extern vector unsigned short __builtin_vec_sr (vector unsigned short a1,
                              vector unsigned short a2);
extern vector signed int __builtin_vec_sr (vector signed int a1, vector unsigned int a2);
extern vector unsigned int __builtin_vec_sr (vector unsigned int a1, vector unsigned int a2);

extern vector signed char __builtin_vec_sra (vector signed char a1, vector unsigned char a2);
extern vector unsigned char __builtin_vec_sra (vector unsigned char a1,
                              vector unsigned char a2);
extern vector signed short __builtin_vec_sra (vector signed short a1,
                             vector unsigned short a2);
extern vector unsigned short __builtin_vec_sra (vector unsigned short a1,
                               vector unsigned short a2);
extern vector signed int __builtin_vec_sra (vector signed int a1, vector unsigned int a2);
extern vector unsigned int __builtin_vec_sra (vector unsigned int a1, vector unsigned int a2);

extern vector signed int __builtin_vec_srl (vector signed int a1, vector unsigned int a2);
extern vector signed int __builtin_vec_srl (vector signed int a1, vector unsigned short a2);
extern vector signed int __builtin_vec_srl (vector signed int a1, vector unsigned char a2);
extern vector unsigned int __builtin_vec_srl (vector unsigned int a1, vector unsigned int a2);
extern vector unsigned int __builtin_vec_srl (vector unsigned int a1,
                             vector unsigned short a2);
extern vector unsigned int __builtin_vec_srl (vector unsigned int a1, vector unsigned char a2);

extern vector signed short __builtin_vec_srl (vector signed short a1, vector unsigned int a2);
extern vector signed short __builtin_vec_srl (vector signed short a1,
                             vector unsigned short a2);
extern vector signed short __builtin_vec_srl (vector signed short a1, vector unsigned char a2);

extern vector unsigned short __builtin_vec_srl (vector unsigned short a1,
                               vector unsigned int a2);
extern vector unsigned short __builtin_vec_srl (vector unsigned short a1,
                               vector unsigned short a2);
extern vector unsigned short __builtin_vec_srl (vector unsigned short a1,
                               vector unsigned char a2);
extern vector signed char __builtin_vec_srl (vector signed char a1, vector unsigned int a2);
extern vector signed char __builtin_vec_srl (vector signed char a1, vector unsigned short a2);
extern vector signed char __builtin_vec_srl (vector signed char a1, vector unsigned char a2);
extern vector unsigned char __builtin_vec_srl (vector unsigned char a1,
                              vector unsigned int a2);
extern vector unsigned char __builtin_vec_srl (vector unsigned char a1,
                              vector unsigned short a2);
extern vector unsigned char __builtin_vec_srl (vector unsigned char a1,
                              vector unsigned char a2);

extern vector float __builtin_vec_sro (vector float a1, vector signed char a2);
extern vector float __builtin_vec_sro (vector float a1, vector unsigned char a2);
extern vector signed int __builtin_vec_sro (vector signed int a1, vector signed char a2);
extern vector signed int __builtin_vec_sro (vector signed int a1, vector unsigned char a2);
extern vector unsigned int __builtin_vec_sro (vector unsigned int a1, vector signed char a2);
extern vector unsigned int __builtin_vec_sro (vector unsigned int a1, vector unsigned char a2);

extern vector signed short __builtin_vec_sro (vector signed short a1, vector signed char a2);
extern vector signed short __builtin_vec_sro (vector signed short a1, vector unsigned char a2);

extern vector unsigned short __builtin_vec_sro (vector unsigned short a1,
                               vector signed char a2);
extern vector unsigned short __builtin_vec_sro (vector unsigned short a1,
                               vector unsigned char a2);
extern vector signed char __builtin_vec_sro (vector signed char a1, vector signed char a2);
extern vector signed char __builtin_vec_sro (vector signed char a1, vector unsigned char a2);
extern vector unsigned char __builtin_vec_sro (vector unsigned char a1, vector signed char a2);

extern vector unsigned char __builtin_vec_sro (vector unsigned char a1,
                              vector unsigned char a2);

extern void __builtin_vec_st (vector float a1, int a2, const void * a3);
extern void __builtin_vec_st (vector signed int a1, int a2, const void * a3);
extern void __builtin_vec_st (vector unsigned int a1, int a2, const void * a3);
extern void __builtin_vec_st (vector signed short a1, int a2, const void * a3);
extern void __builtin_vec_st (vector unsigned short a1, int a2, const void * a3);
extern void __builtin_vec_st (vector signed char a1, int a2, const void * a3);
extern void __builtin_vec_st (vector unsigned char a1, int a2, const void * a3);

extern void __builtin_vec_ste (vector signed char a1, int a2, const void * a3);
extern void __builtin_vec_ste (vector unsigned char a1, int a2, unsigned char * a3);
extern void __builtin_vec_ste (vector signed short a1, int a2, const void * a3);
extern void __builtin_vec_ste (vector unsigned short a1, int a2, const void * a3);
extern void __builtin_vec_ste (vector signed int a1, int a2, const void * a3);
extern void __builtin_vec_ste (vector unsigned int a1, int a2, unsigned int * a3);
extern void __builtin_vec_ste (vector float a1, int a2, float * a3);

extern void __builtin_vec_stl (vector float a1, int a2, const void * a3);
extern void __builtin_vec_stl (vector signed int a1, int a2, const void * a3);
extern void __builtin_vec_stl (vector unsigned int a1, int a2, const void * a3);
extern void __builtin_vec_stl (vector signed short a1, int a2, const void * a3);
extern void __builtin_vec_stl (vector unsigned short a1, int a2, const void * a3);
extern void __builtin_vec_stl (vector signed char a1, int a2, const void * a3);
extern void __builtin_vec_stl (vector unsigned char a1, int a2, const void * a3);

extern vector signed char __builtin_vec_sub (vector signed char a1, vector signed char a2);
extern vector unsigned char __builtin_vec_sub (vector signed char a1, vector unsigned char a2);

extern vector unsigned char __builtin_vec_sub (vector unsigned char a1, vector signed char a2);

extern vector unsigned char __builtin_vec_sub (vector unsigned char a1,
                              vector unsigned char a2);
extern vector signed short __builtin_vec_sub (vector signed short a1, vector signed short a2);
extern vector unsigned short __builtin_vec_sub (vector signed short a1,
                               vector unsigned short a2);
extern vector unsigned short __builtin_vec_sub (vector unsigned short a1,
                               vector signed short a2);
extern vector unsigned short __builtin_vec_sub (vector unsigned short a1,
                               vector unsigned short a2);
extern vector signed int __builtin_vec_sub (vector signed int a1, vector signed int a2);
extern vector unsigned int __builtin_vec_sub (vector signed int a1, vector unsigned int a2);
extern vector unsigned int __builtin_vec_sub (vector unsigned int a1, vector signed int a2);
extern vector unsigned int __builtin_vec_sub (vector unsigned int a1, vector unsigned int a2);
extern vector float __builtin_vec_sub (vector float a1, vector float a2);

extern vector unsigned int __builtin_vec_subc (vector unsigned int a1, vector unsigned int a2);

extern vector unsigned char __builtin_vec_subs (vector signed char a1,
                               vector unsigned char a2);
extern vector unsigned char __builtin_vec_subs (vector unsigned char a1,
                               vector signed char a2);
extern vector unsigned char __builtin_vec_subs (vector unsigned char a1,
                               vector unsigned char a2);
extern vector signed char __builtin_vec_subs (vector signed char a1, vector signed char a2);
extern vector unsigned short __builtin_vec_subs (vector signed short a1,
                                vector unsigned short a2);
extern vector unsigned short __builtin_vec_subs (vector unsigned short a1,
                                vector signed short a2);
extern vector unsigned short __builtin_vec_subs (vector unsigned short a1,
                                vector unsigned short a2);
extern vector signed short __builtin_vec_subs (vector signed short a1, vector signed short a2);

extern vector unsigned int __builtin_vec_subs (vector signed int a1, vector unsigned int a2);
extern vector unsigned int __builtin_vec_subs (vector unsigned int a1, vector signed int a2);
extern vector unsigned int __builtin_vec_subs (vector unsigned int a1, vector unsigned int a2);

extern vector signed int __builtin_vec_subs (vector signed int a1, vector signed int a2);

extern vector unsigned int __builtin_vec_sum4s (vector unsigned char a1,
                               vector unsigned int a2);
extern vector signed int __builtin_vec_sum4s (vector signed char a1, vector signed int a2);
extern vector signed int __builtin_vec_sum4s (vector signed short a1, vector signed int a2);
#else
# define __builtin_vec_sr(a, b) ((a) >> (b))
# define __builtin_vec_sub(a, b) ((a) - (b))
__coverity_generic_altivec_vector __builtin_vec_sra ();
__coverity_generic_altivec_vector __builtin_vec_srl ();
__coverity_generic_altivec_vector __builtin_vec_sro ();
extern void __builtin_vec_st ();
extern void __builtin_vec_ste ();
extern void __builtin_vec_stl ();
__coverity_generic_altivec_vector __builtin_vec_subc ();
__coverity_generic_altivec_vector __builtin_vec_subs ();
__coverity_generic_altivec_vector __builtin_vec_sum4s ();
#endif

extern vector signed int __builtin_vec_sum2s (vector signed int a1, vector signed int a2);

extern vector signed int __builtin_vec_sums (vector signed int a1, vector signed int a2);

extern vector float __builtin_vec_trunc (vector float a1);

#ifdef __coverity_cplusplus
extern vector signed short __builtin_vec_unpackh (vector signed char a1);
extern vector unsigned int __builtin_vec_unpackh (vector unsigned short a1);
extern vector signed int __builtin_vec_unpackh (vector signed short a1);

extern vector signed short __builtin_vec_unpackl (vector signed char a1);
extern vector unsigned int __builtin_vec_unpackl (vector unsigned short a1);
extern vector signed int __builtin_vec_unpackl (vector signed short a1);

extern vector float __builtin_vec_xor (vector float a1, vector float a2);
extern vector float __builtin_vec_xor (vector float a1, vector signed int a2);
extern vector float __builtin_vec_xor (vector signed int a1, vector float a2);
extern vector signed int __builtin_vec_xor (vector signed int a1, vector signed int a2);
extern vector unsigned int __builtin_vec_xor (vector signed int a1, vector unsigned int a2);
extern vector unsigned int __builtin_vec_xor (vector unsigned int a1, vector signed int a2);
extern vector unsigned int __builtin_vec_xor (vector unsigned int a1, vector unsigned int a2);
extern vector signed short __builtin_vec_xor (vector signed short a1, vector signed short a2);
extern vector unsigned short __builtin_vec_xor (vector signed short a1,
                               vector unsigned short a2);
extern vector unsigned short __builtin_vec_xor (vector unsigned short a1,
                               vector signed short a2);
extern vector unsigned short __builtin_vec_xor (vector unsigned short a1,
                               vector unsigned short a2);
extern vector signed char __builtin_vec_xor (vector signed char a1, vector signed char a2);
extern vector unsigned char __builtin_vec_xor (vector signed char a1, vector unsigned char a2);

extern vector unsigned char __builtin_vec_xor (vector unsigned char a1, vector signed char a2);

extern vector unsigned char __builtin_vec_xor (vector unsigned char a1,
                              vector unsigned char a2);

extern vector signed int __builtin_vec_all_eq (vector signed char a1, vector unsigned char a2);

extern vector signed int __builtin_vec_all_eq (vector signed char a1, vector signed char a2);
extern vector signed int __builtin_vec_all_eq (vector unsigned char a1, vector signed char a2);

extern vector signed int __builtin_vec_all_eq (vector unsigned char a1,
                              vector unsigned char a2);
extern vector signed int __builtin_vec_all_eq (vector signed short a1,
                              vector unsigned short a2);
extern vector signed int __builtin_vec_all_eq (vector signed short a1, vector signed short a2);

extern vector signed int __builtin_vec_all_eq (vector unsigned short a1,
                              vector signed short a2);
extern vector signed int __builtin_vec_all_eq (vector unsigned short a1,
                              vector unsigned short a2);
extern vector signed int __builtin_vec_all_eq (vector signed int a1, vector unsigned int a2);
extern vector signed int __builtin_vec_all_eq (vector signed int a1, vector signed int a2);
extern vector signed int __builtin_vec_all_eq (vector unsigned int a1, vector signed int a2);
extern vector signed int __builtin_vec_all_eq (vector unsigned int a1, vector unsigned int a2);

extern vector signed int __builtin_vec_all_eq (vector float a1, vector float a2);

extern vector signed int __builtin_vec_all_ge (vector signed char a1, vector unsigned char a2);

extern vector signed int __builtin_vec_all_ge (vector unsigned char a1, vector signed char a2);

extern vector signed int __builtin_vec_all_ge (vector unsigned char a1,
                              vector unsigned char a2);
extern vector signed int __builtin_vec_all_ge (vector signed char a1, vector signed char a2);
extern vector signed int __builtin_vec_all_ge (vector signed short a1,
                              vector unsigned short a2);
extern vector signed int __builtin_vec_all_ge (vector unsigned short a1,
                              vector signed short a2);
extern vector signed int __builtin_vec_all_ge (vector unsigned short a1,
                              vector unsigned short a2);
extern vector signed int __builtin_vec_all_ge (vector signed short a1, vector signed short a2);

extern vector signed int __builtin_vec_all_ge (vector signed int a1, vector unsigned int a2);
extern vector signed int __builtin_vec_all_ge (vector unsigned int a1, vector signed int a2);
extern vector signed int __builtin_vec_all_ge (vector unsigned int a1, vector unsigned int a2);

extern vector signed int __builtin_vec_all_ge (vector signed int a1, vector signed int a2);
extern vector signed int __builtin_vec_all_ge (vector float a1, vector float a2);

extern vector signed int __builtin_vec_all_gt (vector signed char a1, vector unsigned char a2);

extern vector signed int __builtin_vec_all_gt (vector unsigned char a1, vector signed char a2);

extern vector signed int __builtin_vec_all_gt (vector unsigned char a1,
                              vector unsigned char a2);
extern vector signed int __builtin_vec_all_gt (vector signed char a1, vector signed char a2);
extern vector signed int __builtin_vec_all_gt (vector signed short a1,
                              vector unsigned short a2);
extern vector signed int __builtin_vec_all_gt (vector unsigned short a1,
                              vector signed short a2);
extern vector signed int __builtin_vec_all_gt (vector unsigned short a1,
                              vector unsigned short a2);
extern vector signed int __builtin_vec_all_gt (vector signed short a1, vector signed short a2);

extern vector signed int __builtin_vec_all_gt (vector signed int a1, vector unsigned int a2);
extern vector signed int __builtin_vec_all_gt (vector unsigned int a1, vector signed int a2);
extern vector signed int __builtin_vec_all_gt (vector unsigned int a1, vector unsigned int a2);

extern vector signed int __builtin_vec_all_gt (vector signed int a1, vector signed int a2);
extern vector signed int __builtin_vec_all_gt (vector float a1, vector float a2);

extern vector signed int __builtin_vec_all_in (vector float a1, vector float a2);

extern vector signed int __builtin_vec_all_le (vector signed char a1, vector unsigned char a2);

extern vector signed int __builtin_vec_all_le (vector unsigned char a1, vector signed char a2);

extern vector signed int __builtin_vec_all_le (vector unsigned char a1,
                              vector unsigned char a2);
extern vector signed int __builtin_vec_all_le (vector signed char a1, vector signed char a2);
extern vector signed int __builtin_vec_all_le (vector signed short a1,
                              vector unsigned short a2);
extern vector signed int __builtin_vec_all_le (vector unsigned short a1,
                              vector signed short a2);
extern vector signed int __builtin_vec_all_le (vector unsigned short a1,
                              vector unsigned short a2);
extern vector signed int __builtin_vec_all_le (vector signed short a1, vector signed short a2);

extern vector signed int __builtin_vec_all_le (vector signed int a1, vector unsigned int a2);
extern vector signed int __builtin_vec_all_le (vector unsigned int a1, vector signed int a2);
extern vector signed int __builtin_vec_all_le (vector unsigned int a1, vector unsigned int a2);

extern vector signed int __builtin_vec_all_le (vector signed int a1, vector signed int a2);
extern vector signed int __builtin_vec_all_le (vector float a1, vector float a2);

extern vector signed int __builtin_vec_all_lt (vector signed char a1, vector unsigned char a2);

extern vector signed int __builtin_vec_all_lt (vector unsigned char a1, vector signed char a2);

extern vector signed int __builtin_vec_all_lt (vector unsigned char a1,
                              vector unsigned char a2);
extern vector signed int __builtin_vec_all_lt (vector signed char a1, vector signed char a2);
extern vector signed int __builtin_vec_all_lt (vector signed short a1,
                              vector unsigned short a2);
extern vector signed int __builtin_vec_all_lt (vector unsigned short a1,
                              vector signed short a2);
extern vector signed int __builtin_vec_all_lt (vector unsigned short a1,
                              vector unsigned short a2);
extern vector signed int __builtin_vec_all_lt (vector signed short a1, vector signed short a2);

extern vector signed int __builtin_vec_all_lt (vector signed int a1, vector unsigned int a2);
extern vector signed int __builtin_vec_all_lt (vector unsigned int a1, vector signed int a2);
extern vector signed int __builtin_vec_all_lt (vector unsigned int a1, vector unsigned int a2);

extern vector signed int __builtin_vec_all_lt (vector signed int a1, vector signed int a2);
extern vector signed int __builtin_vec_all_lt (vector float a1, vector float a2);
#else
__coverity_generic_altivec_vector __builtin_vec_unpackh ();
__coverity_generic_altivec_vector __builtin_vec_unpackl ();
#define __builtin_vec_xor(a1, a2) ((a1) ^ (a2))
__coverity_generic_altivec_vector __builtin_vec_all_eq ();
__coverity_generic_altivec_vector __builtin_vec_all_ge ();
__coverity_generic_altivec_vector __builtin_vec_all_gt ();
__coverity_generic_altivec_vector __builtin_vec_all_le ();
__coverity_generic_altivec_vector __builtin_vec_all_lt ();
#endif

extern vector signed int __builtin_vec_all_nan (vector float a1);

#ifdef __coverity_cplusplus
extern vector signed int __builtin_vec_all_ne (vector signed char a1, vector unsigned char a2);

extern vector signed int __builtin_vec_all_ne (vector signed char a1, vector signed char a2);
extern vector signed int __builtin_vec_all_ne (vector unsigned char a1, vector signed char a2);

extern vector signed int __builtin_vec_all_ne (vector unsigned char a1,
                              vector unsigned char a2);
extern vector signed int __builtin_vec_all_ne (vector signed short a1,
                              vector unsigned short a2);
extern vector signed int __builtin_vec_all_ne (vector signed short a1, vector signed short a2);

extern vector signed int __builtin_vec_all_ne (vector unsigned short a1,
                              vector signed short a2);
extern vector signed int __builtin_vec_all_ne (vector unsigned short a1,
                              vector unsigned short a2);
extern vector signed int __builtin_vec_all_ne (vector signed int a1, vector unsigned int a2);
extern vector signed int __builtin_vec_all_ne (vector signed int a1, vector signed int a2);
extern vector signed int __builtin_vec_all_ne (vector unsigned int a1, vector signed int a2);
extern vector signed int __builtin_vec_all_ne (vector unsigned int a1, vector unsigned int a2);

extern vector signed int __builtin_vec_all_ne (vector float a1, vector float a2);

#else
__coverity_generic_altivec_vector __builtin_vec_all_ne ();
#endif

extern vector signed int __builtin_vec_all_nge (vector float a1, vector float a2);

extern vector signed int __builtin_vec_all_ngt (vector float a1, vector float a2);

extern vector signed int __builtin_vec_all_nle (vector float a1, vector float a2);

extern vector signed int __builtin_vec_all_nlt (vector float a1, vector float a2);

extern vector signed int __builtin_vec_all_numeric (vector float a1);

#ifdef __coverity_cplusplus
extern vector signed int __builtin_vec_any_eq (vector signed char a1, vector unsigned char a2);

extern vector signed int __builtin_vec_any_eq (vector signed char a1, vector signed char a2);
extern vector signed int __builtin_vec_any_eq (vector unsigned char a1, vector signed char a2);

extern vector signed int __builtin_vec_any_eq (vector unsigned char a1,
                              vector unsigned char a2);
extern vector signed int __builtin_vec_any_eq (vector signed short a1,
                              vector unsigned short a2);
extern vector signed int __builtin_vec_any_eq (vector signed short a1, vector signed short a2);

extern vector signed int __builtin_vec_any_eq (vector unsigned short a1,
                              vector signed short a2);
extern vector signed int __builtin_vec_any_eq (vector unsigned short a1,
                              vector unsigned short a2);
extern vector signed int __builtin_vec_any_eq (vector signed int a1, vector unsigned int a2);
extern vector signed int __builtin_vec_any_eq (vector signed int a1, vector signed int a2);
extern vector signed int __builtin_vec_any_eq (vector unsigned int a1, vector signed int a2);
extern vector signed int __builtin_vec_any_eq (vector unsigned int a1, vector unsigned int a2);

extern vector signed int __builtin_vec_any_eq (vector float a1, vector float a2);

extern vector signed int __builtin_vec_any_ge (vector signed char a1, vector unsigned char a2);

extern vector signed int __builtin_vec_any_ge (vector unsigned char a1, vector signed char a2);

extern vector signed int __builtin_vec_any_ge (vector unsigned char a1,
                              vector unsigned char a2);
extern vector signed int __builtin_vec_any_ge (vector signed char a1, vector signed char a2);
extern vector signed int __builtin_vec_any_ge (vector signed short a1,
                              vector unsigned short a2);
extern vector signed int __builtin_vec_any_ge (vector unsigned short a1,
                              vector signed short a2);
extern vector signed int __builtin_vec_any_ge (vector unsigned short a1,
                              vector unsigned short a2);
extern vector signed int __builtin_vec_any_ge (vector signed short a1, vector signed short a2);

extern vector signed int __builtin_vec_any_ge (vector signed int a1, vector unsigned int a2);
extern vector signed int __builtin_vec_any_ge (vector unsigned int a1, vector signed int a2);
extern vector signed int __builtin_vec_any_ge (vector unsigned int a1, vector unsigned int a2);

extern vector signed int __builtin_vec_any_ge (vector signed int a1, vector signed int a2);
extern vector signed int __builtin_vec_any_ge (vector float a1, vector float a2);

extern vector signed int __builtin_vec_any_gt (vector signed char a1, vector unsigned char a2);

extern vector signed int __builtin_vec_any_gt (vector unsigned char a1, vector signed char a2);

extern vector signed int __builtin_vec_any_gt (vector unsigned char a1,
                              vector unsigned char a2);
extern vector signed int __builtin_vec_any_gt (vector signed char a1, vector signed char a2);
extern vector signed int __builtin_vec_any_gt (vector signed short a1,
                              vector unsigned short a2);
extern vector signed int __builtin_vec_any_gt (vector unsigned short a1,
                              vector signed short a2);
extern vector signed int __builtin_vec_any_gt (vector unsigned short a1,
                              vector unsigned short a2);
extern vector signed int __builtin_vec_any_gt (vector signed short a1, vector signed short a2);

extern vector signed int __builtin_vec_any_gt (vector signed int a1, vector unsigned int a2);
extern vector signed int __builtin_vec_any_gt (vector unsigned int a1, vector signed int a2);
extern vector signed int __builtin_vec_any_gt (vector unsigned int a1, vector unsigned int a2);

extern vector signed int __builtin_vec_any_gt (vector signed int a1, vector signed int a2);
extern vector signed int __builtin_vec_any_gt (vector float a1, vector float a2);

extern vector signed int __builtin_vec_any_le (vector signed char a1, vector unsigned char a2);

extern vector signed int __builtin_vec_any_le (vector unsigned char a1, vector signed char a2);

extern vector signed int __builtin_vec_any_le (vector unsigned char a1,
                              vector unsigned char a2);
extern vector signed int __builtin_vec_any_le (vector signed char a1, vector signed char a2);
extern vector signed int __builtin_vec_any_le (vector signed short a1,
                              vector unsigned short a2);
extern vector signed int __builtin_vec_any_le (vector unsigned short a1,
                              vector signed short a2);
extern vector signed int __builtin_vec_any_le (vector unsigned short a1,
                              vector unsigned short a2);
extern vector signed int __builtin_vec_any_le (vector signed short a1, vector signed short a2);

extern vector signed int __builtin_vec_any_le (vector signed int a1, vector unsigned int a2);
extern vector signed int __builtin_vec_any_le (vector unsigned int a1, vector signed int a2);
extern vector signed int __builtin_vec_any_le (vector unsigned int a1, vector unsigned int a2);

extern vector signed int __builtin_vec_any_le (vector signed int a1, vector signed int a2);
extern vector signed int __builtin_vec_any_le (vector float a1, vector float a2);

extern vector signed int __builtin_vec_any_lt (vector signed char a1, vector unsigned char a2);

extern vector signed int __builtin_vec_any_lt (vector unsigned char a1, vector signed char a2);

extern vector signed int __builtin_vec_any_lt (vector unsigned char a1,
                              vector unsigned char a2);
extern vector signed int __builtin_vec_any_lt (vector signed char a1, vector signed char a2);
extern vector signed int __builtin_vec_any_lt (vector signed short a1,
                              vector unsigned short a2);
extern vector signed int __builtin_vec_any_lt (vector unsigned short a1,
                              vector signed short a2);
extern vector signed int __builtin_vec_any_lt (vector unsigned short a1,
                              vector unsigned short a2);
extern vector signed int __builtin_vec_any_lt (vector signed short a1, vector signed short a2);

extern vector signed int __builtin_vec_any_lt (vector signed int a1, vector unsigned int a2);
extern vector signed int __builtin_vec_any_lt (vector unsigned int a1, vector signed int a2);
extern vector signed int __builtin_vec_any_lt (vector unsigned int a1, vector unsigned int a2);

extern vector signed int __builtin_vec_any_lt (vector signed int a1, vector signed int a2);
extern vector signed int __builtin_vec_any_lt (vector float a1, vector float a2);
#else
__coverity_generic_altivec_vector __builtin_vec_any_eq ();
__coverity_generic_altivec_vector __builtin_vec_any_ge ();
__coverity_generic_altivec_vector __builtin_vec_any_gt ();
__coverity_generic_altivec_vector __builtin_vec_any_le ();
__coverity_generic_altivec_vector __builtin_vec_any_lt ();
#endif

extern vector signed int __builtin_vec_any_nan (vector float a1);

#ifdef __coverity_cplusplus
extern vector signed int __builtin_vec_any_ne (vector signed char a1, vector unsigned char a2);

extern vector signed int __builtin_vec_any_ne (vector signed char a1, vector signed char a2);
extern vector signed int __builtin_vec_any_ne (vector unsigned char a1, vector signed char a2);

extern vector signed int __builtin_vec_any_ne (vector unsigned char a1,
                              vector unsigned char a2);
extern vector signed int __builtin_vec_any_ne (vector signed short a1,
                              vector unsigned short a2);
extern vector signed int __builtin_vec_any_ne (vector signed short a1, vector signed short a2);

extern vector signed int __builtin_vec_any_ne (vector unsigned short a1,
                              vector signed short a2);
extern vector signed int __builtin_vec_any_ne (vector unsigned short a1,
                              vector unsigned short a2);
extern vector signed int __builtin_vec_any_ne (vector signed int a1, vector unsigned int a2);
extern vector signed int __builtin_vec_any_ne (vector signed int a1, vector signed int a2);
extern vector signed int __builtin_vec_any_ne (vector unsigned int a1, vector signed int a2);
extern vector signed int __builtin_vec_any_ne (vector unsigned int a1, vector unsigned int a2);

extern vector signed int __builtin_vec_any_ne (vector float a1, vector float a2);
#else
__coverity_generic_altivec_vector __builtin_vec_any_ne ();
#endif

extern vector signed int __builtin_vec_any_nge (vector float a1, vector float a2);

extern vector signed int __builtin_vec_any_ngt (vector float a1, vector float a2);

extern vector signed int __builtin_vec_any_nle (vector float a1, vector float a2);

extern vector signed int __builtin_vec_any_nlt (vector float a1, vector float a2);

extern vector signed int __builtin_vec_any_numeric (vector float a1);

extern vector signed int __builtin_vec_any_out (vector float a1, vector float a2);

# ifndef OFFSET_T
#  ifdef _ARCH_PPC64
#   define OFFSET_T long
#  else
#   define OFFSET_T int
# endif
# endif

extern int __builtin_altivec_vcmpbfp_p(int a1, vector float a2, vector float a3);
extern int __builtin_altivec_vcmpeqfp_p(int a1, vector float a2, vector float a3);
extern int __builtin_altivec_vcmpeqfp_p(int, vector float a1, vector float a2);
extern int __builtin_altivec_vcmpequb_p(int a1, vector signed char a2, vector signed char a3);
extern int __builtin_altivec_vcmpequh_p(int a1, vector signed short a2, vector signed short a3);
extern int __builtin_altivec_vcmpequw_p(int a1, vector signed int a2, vector signed int a3);
extern int __builtin_altivec_vcmpgefp_p(int a1, vector float a2, vector float a3);
extern int __builtin_altivec_vcmpgtfp_p(int a1, vector float a2, vector float a3);
extern int __builtin_altivec_vcmpgtsb_p(int a1, vector signed char a2, vector signed char a3);
extern int __builtin_altivec_vcmpgtsh_p(int a1, vector signed short a2, vector signed short a3);
extern int __builtin_altivec_vcmpgtsw_p(int a1, vector signed int a2, vector signed int a3);
extern int __builtin_altivec_vcmpgtub_p(int a1, vector signed char a2, vector signed char a3);
extern int __builtin_altivec_vcmpgtuh_p(int a1, vector signed short a2, vector signed short a3);
extern int __builtin_altivec_vcmpgtuw_p(int a1, vector signed int a2, vector signed int a3);
extern vector bool char __builtin_altivec_lvx(OFFSET_T a1, const void * a2);
extern vector bool char __builtin_altivec_lvxl(OFFSET_T a1, const void * a2);
extern vector bool char __builtin_altivec_vand(vector signed int a1, vector signed int a2);
extern vector bool char __builtin_altivec_vandc(vector signed int a1, vector signed int a2);
extern vector bool char __builtin_altivec_vcmpequb(vector signed char a1, vector signed char a2);
extern vector bool char __builtin_altivec_vcmpgtsb(vector signed char a1, vector signed char a2);
extern vector bool char __builtin_altivec_vcmpgtub(vector signed char a1, vector signed char a2);
extern vector bool char __builtin_altivec_vmrghb(vector signed char a1, vector signed char a2);
extern vector bool char __builtin_altivec_vmrglb(vector signed char a1, vector signed char a2);
extern vector bool char __builtin_altivec_vnor(vector signed int a1, vector signed int a2);
extern vector bool char __builtin_altivec_vor(vector signed int a1, vector signed int a2);
extern vector bool char __builtin_altivec_vperm_4si(vector signed int a1, vector signed int a2, vector signed char a3);
extern vector bool char __builtin_altivec_vpkuhum(vector signed short a1, vector signed short a2);
extern vector bool char __builtin_altivec_vsel_4si(vector signed int a1, vector signed int a2, vector signed int a3);
extern vector bool char __builtin_altivec_vsl(vector signed int a1, vector signed int a2);
extern vector bool char __builtin_altivec_vsldoi_4si(vector signed int a1, vector signed int a2, const int a3);
extern vector bool char __builtin_altivec_vsldoi_4sf(vector float a1, vector float a2, const int a3);
extern vector bool char __builtin_altivec_vspltb(vector signed char a1, const int a2);
extern vector bool char __builtin_altivec_vsr(vector signed int a1, vector signed int a2);
extern vector bool char __builtin_altivec_vxor(vector signed int a1, vector signed int a2);
extern vector bool int __builtin_altivec_vcmpeqfp(vector float a1, vector float a2);
extern vector bool int __builtin_altivec_vcmpequw(vector signed int a1, vector signed int a2);
extern vector bool int __builtin_altivec_vcmpgefp(vector float a1, vector float a2);
extern vector bool int __builtin_altivec_vcmpgtfp(vector float a1, vector float a2);
extern vector bool int __builtin_altivec_vcmpgtsw(vector signed int a1, vector signed int a2);
extern vector bool int __builtin_altivec_vcmpgtuw(vector signed int a1, vector signed int a2);
extern vector bool int __builtin_altivec_vmrghw(vector signed int a1, vector signed int a2);
extern vector bool int __builtin_altivec_vmrglw(vector signed int a1, vector signed int a2);
extern vector bool int __builtin_altivec_vspltw(vector signed int a1, const int a2);
extern vector bool int __builtin_altivec_vupkhsh(vector signed short a1);
extern vector bool int __builtin_altivec_vupklsh(vector signed short a1);
extern vector bool short __builtin_altivec_vcmpequh(vector signed short a1, vector signed short a2);
extern vector bool short __builtin_altivec_vcmpgtsh(vector signed short a1, vector signed short a2);
extern vector bool short __builtin_altivec_vcmpgtuh(vector signed short a1, vector signed short a2);
extern vector bool short __builtin_altivec_vmrghh(vector signed short a1, vector signed short a2);
extern vector bool short __builtin_altivec_vmrglh(vector signed short a1, vector signed short a2);
extern vector bool short __builtin_altivec_vpkuwum(vector signed int a1, vector signed int a2);
extern vector bool short __builtin_altivec_vsplth(vector signed short a1, const int a2);
extern vector bool short __builtin_altivec_vupkhsb(vector signed char a1);
extern vector bool short __builtin_altivec_vupklsb(vector signed char a1);
extern vector float __builtin_altivec_abs_v4sf(vector float a1);
extern vector float __builtin_altivec_lvewx(OFFSET_T a1, const void * a2);
extern vector float __builtin_altivec_vaddfp(vector float a1, vector float a2);
extern vector float __builtin_altivec_vcfsx(vector signed int a1, const int a2);
extern vector float __builtin_altivec_vcfux(vector signed int a1, const int a2);
extern vector float __builtin_altivec_vexptefp(vector float a1);
extern vector float __builtin_altivec_vlogefp(vector float a1);
extern vector float __builtin_altivec_vmaddfp(vector float a1, vector float a2, vector float a3);
extern vector float __builtin_altivec_vmaxfp(vector float a1, vector float a2);
extern vector float __builtin_altivec_vminfp(vector float a1, vector float a2);
extern vector float __builtin_altivec_vnmsubfp(vector float a1, vector float a2, vector float a3);
extern vector float __builtin_altivec_vrefp(vector float a1);
extern vector float __builtin_altivec_vrfim(vector float a1);
extern vector float __builtin_altivec_vrfin(vector float a1);
extern vector float __builtin_altivec_vrfip(vector float a1);
extern vector float __builtin_altivec_vrfiz(vector float a1);
extern vector float __builtin_altivec_vrsqrtefp(vector float a1);
extern vector float __builtin_altivec_vslo(vector signed int a1, vector signed int a2);
extern vector float __builtin_altivec_vsro(vector signed int a1, vector signed int a2);
extern vector float __builtin_altivec_vsubfp(vector float a1, vector float a2);
extern vector pixel __builtin_altivec_vpkpx(vector signed int a1, vector signed int a2);
extern vector signed char __builtin_altivec_abs_v16qi(vector signed char a1);
extern vector signed char __builtin_altivec_abss_v16qi(vector signed char a1);
extern vector signed char __builtin_altivec_lvebx(OFFSET_T a1, const void * a2);
extern vector signed char __builtin_altivec_vaddsbs(vector signed char a1, vector signed char a2);
extern vector signed char __builtin_altivec_vaddubm(vector signed char a1, vector signed char a2);
extern vector signed char __builtin_altivec_vavgsb(vector signed char a1, vector signed char a2);
extern vector signed char __builtin_altivec_vmaxsb(vector signed char a1, vector signed char a2);
extern vector signed char __builtin_altivec_vminsb(vector signed char a1, vector signed char a2);
extern vector signed char __builtin_altivec_vpkshss(vector signed short a1, vector signed short a2);
extern vector signed char __builtin_altivec_vrlb(vector signed char a1, vector signed char a2);
extern vector signed char __builtin_altivec_vslb(vector signed char a1, vector signed char a2);
extern vector signed char __builtin_altivec_vspltisb(const int a1);
extern vector signed char __builtin_altivec_vsrab(vector signed char a1, vector signed char a2);
extern vector signed char __builtin_altivec_vsrb(vector signed char a1, vector signed char a2);
extern vector signed char __builtin_altivec_vsubsbs(vector signed char a1, vector signed char a2);
extern vector signed char __builtin_altivec_vsububm(vector signed char a1, vector signed char a2);
extern vector signed int __builtin_altivec_abs_v4si(vector signed int a1);
extern vector signed int __builtin_altivec_abss_v4si(vector signed int a1);
extern vector signed int __builtin_altivec_vaddsws(vector signed int a1, vector signed int a2);
extern vector signed int __builtin_altivec_vadduwm(vector signed int a1, vector signed int a2);
extern vector signed int __builtin_altivec_vavgsw(vector signed int a1, vector signed int a2);
extern vector signed int __builtin_altivec_vcmpbfp(vector float a1, vector float a2);
extern vector signed int __builtin_altivec_vctsxs(vector float a1, const int a2);
extern vector signed int __builtin_altivec_vmaxsw(vector signed int a1, vector signed int a2);
extern vector signed int __builtin_altivec_vminsw(vector signed int a1, vector signed int a2);
extern vector signed int __builtin_altivec_vmsummbm(vector signed char a1, vector signed char a2, vector signed int a3);
extern vector signed int __builtin_altivec_vmsumshm(vector signed short a1, vector signed short a2, vector signed int a3);
extern vector signed int __builtin_altivec_vmsumshs(vector signed short a1, vector signed short a2, vector signed int a3);
extern vector signed int __builtin_altivec_vmulesh(vector signed short a1, vector signed short a2);
extern vector signed int __builtin_altivec_vmulosh(vector signed short a1, vector signed short a2);
extern vector signed int __builtin_altivec_vrlw(vector signed int a1, vector signed int a2);
extern vector signed int __builtin_altivec_vslw(vector signed int a1, vector signed int a2);
extern vector signed int __builtin_altivec_vspltisw(const int a1);
extern vector signed int __builtin_altivec_vsraw(vector signed int a1, vector signed int a2);
extern vector signed int __builtin_altivec_vsrw(vector signed int a1, vector signed int a2);
extern vector signed int __builtin_altivec_vsubsws(vector signed int a1, vector signed int a2);
extern vector signed int __builtin_altivec_vsubuwm(vector signed int a1, vector signed int a2);
extern vector signed int __builtin_altivec_vsum2sws(vector signed int a1, vector signed int a2);
extern vector signed int __builtin_altivec_vsum4sbs(vector signed char a1, vector signed int a2);
extern vector signed int __builtin_altivec_vsum4shs(vector signed short a1, vector signed int a2);
extern vector signed int __builtin_altivec_vsumsws(vector signed int a1, vector signed int a2);
extern vector signed short __builtin_altivec_abs_v8hi(vector signed short a1);
extern vector signed short __builtin_altivec_abss_v8hi(vector signed short a1);
extern vector signed short __builtin_altivec_lvehx(OFFSET_T a1, const void * a2);
extern vector signed short __builtin_altivec_vaddshs(vector signed short a1, vector signed short a2);
extern vector signed short __builtin_altivec_vadduhm(vector signed short a1, vector signed short a2);
extern vector signed short __builtin_altivec_vavgsh(vector signed short a1, vector signed short a2);
extern vector signed short __builtin_altivec_vmaxsh(vector signed short a1, vector signed short a2);
extern vector signed short __builtin_altivec_vmhaddshs(vector signed short a1, vector signed short a2, vector signed short a3);
extern vector signed short __builtin_altivec_vmhraddshs(vector signed short a1, vector signed short a2, vector signed short a3);
extern vector signed short __builtin_altivec_vminsh(vector signed short a1, vector signed short a2);
extern vector signed short __builtin_altivec_vmladduhm(vector signed short a1, vector signed short a2, vector signed short a3);
extern vector signed short __builtin_altivec_vmulesb(vector signed char a1, vector signed char a2);
extern vector signed short __builtin_altivec_vmuleub(vector signed char a1, vector signed char a2);
extern vector signed short __builtin_altivec_vmulosb(vector signed char a1, vector signed char a2);
extern vector signed short __builtin_altivec_vpkswss(vector signed int a1, vector signed int a2);
extern vector signed short __builtin_altivec_vrlh(vector signed short a1, vector signed short a2);
extern vector signed short __builtin_altivec_vslh(vector signed short a1, vector signed short a2);
extern vector signed short __builtin_altivec_vspltish(const int a1);
extern vector signed short __builtin_altivec_vsrah(vector signed short a1, vector signed short a2);
extern vector signed short __builtin_altivec_vsrh(vector signed short a1, vector signed short a2);
extern vector signed short __builtin_altivec_vsubshs(vector signed short a1, vector signed short a2);
extern vector signed short __builtin_altivec_vsubuhm(vector signed short a1, vector signed short a2);
extern vector unsigned char __builtin_altivec_lvlx(OFFSET_T a1, const void * a2);
extern vector unsigned char __builtin_altivec_lvlxl(OFFSET_T a1, const void * a2);
extern vector unsigned char __builtin_altivec_lvrx(OFFSET_T a1, const void * a2);
extern vector unsigned char __builtin_altivec_lvrxl(OFFSET_T a1, const void * a2);
extern vector unsigned char __builtin_altivec_lvsl(OFFSET_T a1, const void * a2);
extern vector unsigned char __builtin_altivec_lvsr(OFFSET_T a1, const void * a2);
extern vector unsigned char __builtin_altivec_vaddubs(vector signed char a1, vector signed char a2);
extern vector unsigned char __builtin_altivec_vavgub(vector signed char a1, vector signed char a2);
extern vector unsigned char __builtin_altivec_vmaxub(vector signed char a1, vector signed char a2);
extern vector unsigned char __builtin_altivec_vminub(vector signed char a1, vector signed char a2);
extern vector unsigned char __builtin_altivec_vpkshus(vector signed short a1, vector signed short a2);
extern vector unsigned char __builtin_altivec_vpkuhus(vector signed short a1, vector signed short a2);
extern vector unsigned char __builtin_altivec_vsububs(vector signed char a1, vector signed char a2);
extern vector unsigned int __builtin_altivec_vaddcuw(vector signed int a1, vector signed int a2);
extern vector unsigned int __builtin_altivec_vadduws(vector signed int a1, vector signed int a2);
extern vector unsigned int __builtin_altivec_vavguw(vector signed int a1, vector signed int a2);
extern vector unsigned int __builtin_altivec_vctuxs(vector float a1, const int a2);
extern vector unsigned int __builtin_altivec_vmaxuw(vector signed int a1, vector signed int a2);
extern vector unsigned int __builtin_altivec_vminuw(vector signed int a1, vector signed int a2);
extern vector unsigned int __builtin_altivec_vmsumubm(vector signed char a1, vector signed char a2, vector signed int a3);
extern vector unsigned int __builtin_altivec_vmsumuhm(vector signed short a1, vector signed short a2, vector signed int a3);
extern vector unsigned int __builtin_altivec_vmsumuhs(vector signed short a1, vector signed short a2, vector signed int a3);
extern vector unsigned int __builtin_altivec_vmuleuh(vector signed short a1, vector signed short a2);
extern vector unsigned int __builtin_altivec_vmulouh(vector signed short a1, vector signed short a2);
extern vector unsigned int __builtin_altivec_vsubcuw(vector signed int a1, vector signed int a2);
extern vector unsigned int __builtin_altivec_vsubuws(vector signed int a1, vector signed int a2);
extern vector unsigned int __builtin_altivec_vsum4ubs(vector signed char a1, vector signed int a2);
extern vector unsigned int __builtin_altivec_vupkhpx(vector signed short a1);
extern vector unsigned int __builtin_altivec_vupklpx(vector signed short a1);
extern vector unsigned short __builtin_altivec_mfvscr();
extern vector unsigned short __builtin_altivec_vadduhs(vector signed short a1, vector signed short a2);
extern vector unsigned short __builtin_altivec_vavguh(vector signed short a1, vector signed short a2);
extern vector unsigned short __builtin_altivec_vmaxuh(vector signed short a1, vector signed short a2);
extern vector unsigned short __builtin_altivec_vminuh(vector signed short a1, vector signed short a2);
extern vector unsigned short __builtin_altivec_vmuloub(vector signed char a1, vector signed char a2);
extern vector unsigned short __builtin_altivec_vpkswus(vector signed int a1, vector signed int a2);
extern vector unsigned short __builtin_altivec_vpkuwus(vector signed int a1, vector signed int a2);
extern vector unsigned short __builtin_altivec_vsubuhs(vector signed short a1, vector signed short a2);
extern void __builtin_altivec_dss(int a1);
extern void __builtin_altivec_dssall();
extern void __builtin_altivec_dst(const void *a1, OFFSET_T a2, const int a3);
extern void __builtin_altivec_dstst(const void *a1, OFFSET_T a2, const int a3);
extern void __builtin_altivec_dststt(const void *a1, OFFSET_T a2, const int a3);
extern void __builtin_altivec_dstt(const void *a1, OFFSET_T a2, const int a3);
extern void __builtin_altivec_mtvscr(vector signed int a1);
#ifdef __coverity_cplusplus
extern void __builtin_altivec_stvebx(vector signed char a1, OFFSET_T a2, const void * a3);
extern void __builtin_altivec_stvehx(vector signed short a1, OFFSET_T a2, const void * a3);
extern void __builtin_altivec_stvewx(vector signed int a1, OFFSET_T a2, const void * a3);
extern void __builtin_altivec_stvlx(vector signed char a1, OFFSET_T a2, const void * a3);
extern void __builtin_altivec_stvlxl(vector signed char a1, OFFSET_T a2, const void * a3);
extern void __builtin_altivec_stvrx(vector signed char a1, OFFSET_T a2, const void * a3);
extern void __builtin_altivec_stvrxl(vector signed char a1, OFFSET_T a2, const void * a3);
extern void __builtin_altivec_stvx(vector signed int a1, OFFSET_T a2, const void * a3);
extern void __builtin_altivec_stvxl(vector signed int a1, OFFSET_T a2, const void * a3);
extern vector bool char __builtin_altivec_vsel_4sf(vector float a1, vector float a2, vector float a3);
extern vector bool char __builtin_altivec_vsel_4sf(vector float a1, vector float a2, vector signed int a3);
#else
extern void __builtin_altivec_stvebx();
extern void __builtin_altivec_stvehx();
extern void __builtin_altivec_stvewx();
extern void __builtin_altivec_stvlx();
extern void __builtin_altivec_stvlxl();
extern void __builtin_altivec_stvrx();
extern void __builtin_altivec_stvrxl();
extern void __builtin_altivec_stvx();
extern void __builtin_altivec_stvxl();
extern __coverity_generic_altivec_vector __builtin_altivec_vsel_4sf();
#endif
#endif /* __ALTIVEC__ */
/*
  Copyright (c) 2023 Synopsys, Inc. All rights reserved worldwide.
  The information contained in this file is the proprietary and confidential
  information of Synopsys, Inc. and its licensors, and is supplied subject to,
  and may be used only by Synopsys customers in accordance with the terms and
  conditions of a previously executed license agreement between Synopsys and
  that customer.
*/

/*
 * These declarations are used to implement various sanitizer modes. They are
 * not supported intrinsically by cov-emit.
 */

// -fsanitize-coverage=trace-cmp
void __asan_after_dynamic_init(void);
void __asan_handle_no_return(void);
void __asan_init(void);
void __asan_version_mismatch_check_v8(void);
void __builtin___asan_after_dynamic_init(void);
void __builtin___asan_handle_no_return(void);
void __builtin___asan_init(void);
void __builtin___asan_version_mismatch_check_v8(void);
void __builtin___sanitizer_cov_trace_pc(void);
void __builtin___tsan_init(void);
void __sanitizer_cov_trace_pc(void);
void __tsan_init(void);
void __asan_alloca_poison(void*, long int);
void __asan_allocas_unpoison(void*, long int);
void __asan_before_dynamic_init(const void*);
void __asan_load1(void*);
void __asan_load16(void*);
void __asan_load16_noabort(void*);
void __asan_load1_noabort(void*);
void __asan_load2(void*);
void __asan_load2_noabort(void*);
void __asan_load4(void*);
void __asan_load4_noabort(void*);
void __asan_load8(void*);
void __asan_load8_noabort(void*);
void __asan_loadN(void*, long int);
void __asan_loadN_noabort(void*, long int);
void __asan_poison_stack_memory(void*, long int);
void __asan_register_globals(void*, long int);
void __asan_report_load1(void*);
void __asan_report_load16(void*);
void __asan_report_load16_noabort(void*);
void __asan_report_load1_noabort(void*);
void __asan_report_load2(void*);
void __asan_report_load2_noabort(void*);
void __asan_report_load4(void*);
void __asan_report_load4_noabort(void*);
void __asan_report_load8(void*);
void __asan_report_load8_noabort(void*);
void __asan_report_load_n(void*, long int);
void __asan_report_load_n_noabort(void*, long int);
void __asan_report_store1(void*);
void __asan_report_store16(void*);
void __asan_report_store16_noabort(void*);
void __asan_report_store1_noabort(void*);
void __asan_report_store2(void*);
void __asan_report_store2_noabort(void*);
void __asan_report_store4(void*);
void __asan_report_store4_noabort(void*);
void __asan_report_store8(void*);
void __asan_report_store8_noabort(void*);
void __asan_report_store_n(void*, long int);
void __asan_report_store_n_noabort(void*, long int);
void __asan_store1(void*);
void __asan_store16(void*);
void __asan_store16_noabort(void*);
void __asan_store1_noabort(void*);
void __asan_store2(void*);
void __asan_store2_noabort(void*);
void __asan_store4(void*);
void __asan_store4_noabort(void*);
void __asan_store8(void*);
void __asan_store8_noabort(void*);
void __asan_storeN(void*, long int);
void __asan_storeN_noabort(void*, long int);
void __asan_unpoison_stack_memory(void*, long int);
void __asan_unregister_globals(void*, long int);
void __builtin___asan_alloca_poison(void*, long int);
void __builtin___asan_allocas_unpoison(void*, long int);
void __builtin___asan_before_dynamic_init(const void*);
void __builtin___asan_load1(void*);
void __builtin___asan_load16(void*);
void __builtin___asan_load16_noabort(void*);
void __builtin___asan_load1_noabort(void*);
void __builtin___asan_load2(void*);
void __builtin___asan_load2_noabort(void*);
void __builtin___asan_load4(void*);
void __builtin___asan_load4_noabort(void*);
void __builtin___asan_load8(void*);
void __builtin___asan_load8_noabort(void*);
void __builtin___asan_loadN(void*, long int);
void __builtin___asan_loadN_noabort(void*, long int);
void __builtin___asan_poison_stack_memory(void*, long int);
void __builtin___asan_register_globals(void*, long int);
void __builtin___asan_report_load1(void*);
void __builtin___asan_report_load16(void*);
void __builtin___asan_report_load16_noabort(void*);
void __builtin___asan_report_load1_noabort(void*);
void __builtin___asan_report_load2(void*);
void __builtin___asan_report_load2_noabort(void*);
void __builtin___asan_report_load4(void*);
void __builtin___asan_report_load4_noabort(void*);
void __builtin___asan_report_load8(void*);
void __builtin___asan_report_load8_noabort(void*);
void __builtin___asan_report_load_n(void*, long int);
void __builtin___asan_report_load_n_noabort(void*, long int);
void __builtin___asan_report_store1(void*);
void __builtin___asan_report_store16(void*);
void __builtin___asan_report_store16_noabort(void*);
void __builtin___asan_report_store1_noabort(void*);
void __builtin___asan_report_store2(void*);
void __builtin___asan_report_store2_noabort(void*);
void __builtin___asan_report_store4(void*);
void __builtin___asan_report_store4_noabort(void*);
void __builtin___asan_report_store8(void*);
void __builtin___sanitizer_cov_trace_cmp2(short unsigned int, short unsigned int);
void __builtin___sanitizer_cov_trace_cmp4(unsigned int, unsigned int);
void __builtin___sanitizer_cov_trace_cmp8(long unsigned int, long unsigned int);
void __builtin___sanitizer_cov_trace_cmpd(double, double);
void __builtin___sanitizer_cov_trace_cmpf(float, float);
void __builtin___sanitizer_cov_trace_const_cmp1(unsigned char, unsigned char);
void __builtin___sanitizer_cov_trace_const_cmp2(short unsigned int, short unsigned int);
void __builtin___sanitizer_cov_trace_const_cmp4(unsigned int, unsigned int);
void __builtin___sanitizer_cov_trace_const_cmp8(long unsigned int, long unsigned int);
void __builtin___sanitizer_cov_trace_switch(long unsigned int, void*);
void __builtin___sanitizer_ptr_cmp(void*, long int);
void __builtin___sanitizer_ptr_sub(void*, long int);

void __builtin___hwasan_handle_longjmp(const void*);
void __builtin___hwasan_load1(void*);
void __builtin___hwasan_load16(void*);
void __builtin___hwasan_load16_noabort(void*);
void __builtin___hwasan_load1_noabort(void*);
void __builtin___hwasan_load2(void*);
void __builtin___hwasan_load2_noabort(void*);
void __builtin___hwasan_load4(void*);
void __builtin___hwasan_load4_noabort(void*);
void __builtin___hwasan_load8(void*);
void __builtin___hwasan_load8_noabort(void*);
void __builtin___hwasan_loadN(void*, long int);
void __builtin___hwasan_loadN_noabort(void*, long int);
void __builtin___hwasan_store1(void*);
void __builtin___hwasan_store16(void*);
void __builtin___hwasan_store16_noabort(void*);
void __builtin___hwasan_store1_noabort(void*);
void __builtin___hwasan_store2(void*);
void __builtin___hwasan_store2_noabort(void*);
void __builtin___hwasan_store4(void*);
void __builtin___hwasan_store4_noabort(void*);
void __builtin___hwasan_store8(void*);
void __builtin___hwasan_store8_noabort(void*);
void __builtin___hwasan_storeN(void*, long int);
void __builtin___hwasan_storeN_noabort(void*, long int);
void __builtin___hwasan_tag_memory(void*, unsigned char, long int);
void __builtin___hwasan_tag_mismatch4(void*);
void* __builtin___hwasan_tag_pointer(const void*, unsigned char);
void __builtin___tsan_volatile_read1(void*);
void __builtin___tsan_volatile_read16(void*);
void __builtin___tsan_volatile_read2(void*);
void __builtin___tsan_volatile_read4(void*);
void __builtin___tsan_volatile_read8(void*);
void __builtin___tsan_volatile_write1(void*);
void __builtin___tsan_volatile_write16(void*);
void __builtin___tsan_volatile_write2(void*);
void __builtin___tsan_volatile_write4(void*);
void __builtin___tsan_volatile_write8(void*);
void __hwasan_handle_longjmp(const void*);
void __hwasan_load1(void*);
void __hwasan_load16(void*);
void __hwasan_load16_noabort(void*);
void __hwasan_load1_noabort(void*);
void __hwasan_load2(void*);
void __hwasan_load2_noabort(void*);
void __hwasan_load4(void*);
void __hwasan_load4_noabort(void*);
void __hwasan_load8(void*);
void __hwasan_load8_noabort(void*);
void __hwasan_loadN(void*, long int);
void __hwasan_loadN_noabort(void*, long int);
void __hwasan_store1(void*);
void __hwasan_store16(void*);
void __hwasan_store16_noabort(void*);
void __hwasan_store1_noabort(void*);
void __hwasan_store2(void*);
void __hwasan_store2_noabort(void*);
void __hwasan_store4(void*);
void __hwasan_store4_noabort(void*);
void __hwasan_store8(void*);
void __hwasan_store8_noabort(void*);
void __hwasan_storeN(void*, long int);
void __hwasan_storeN_noabort(void*, long int);
void __hwasan_tag_memory(void*, unsigned char, long int);
void __hwasan_tag_mismatch4(void*);
void* __hwasan_tag_pointer(const void*, unsigned char);
void __tsan_volatile_read1(void*);
void __tsan_volatile_read16(void*);
void __tsan_volatile_read2(void*);
void __tsan_volatile_read4(void*);
void __tsan_volatile_read8(void*);
void __tsan_volatile_write1(void*);
void __tsan_volatile_write16(void*);
void __tsan_volatile_write2(void*);
void __tsan_volatile_write4(void*);
void __tsan_volatile_write8(void*);
void __builtin___hwasan_init(void);
void __hwasan_init(void);

#if defined(__SIZEOF_INT128__)
__edg_bool_type__ __builtin___tsan_atomic128_compare_exchange_strong(volatile void*, void*, __int128 unsigned, int, int);
__edg_bool_type__ __builtin___tsan_atomic128_compare_exchange_weak(volatile void*, void*, __int128 unsigned, int, int);
__int128 unsigned __builtin___tsan_atomic128_exchange(volatile void*, __int128 unsigned, int);
__int128 unsigned __builtin___tsan_atomic128_fetch_add(volatile void*, __int128 unsigned, int);
__int128 unsigned __builtin___tsan_atomic128_fetch_and(volatile void*, __int128 unsigned, int);
__int128 unsigned __builtin___tsan_atomic128_fetch_nand(volatile void*, __int128 unsigned, int);
__int128 unsigned __builtin___tsan_atomic128_fetch_or(volatile void*, __int128 unsigned, int);
__int128 unsigned __builtin___tsan_atomic128_fetch_sub(volatile void*, __int128 unsigned, int);
__int128 unsigned __builtin___tsan_atomic128_fetch_xor(volatile void*, __int128 unsigned, int);
__int128 unsigned __builtin___tsan_atomic128_load(const volatile void*, int);
void __builtin___tsan_atomic128_store(volatile void*, __int128 unsigned, int);
__edg_bool_type__ __tsan_atomic128_compare_exchange_strong(volatile void*, void*, __int128 unsigned, int, int);
__edg_bool_type__ __tsan_atomic128_compare_exchange_weak(volatile void*, void*, __int128 unsigned, int, int);
__int128 unsigned __tsan_atomic128_exchange(volatile void*, __int128 unsigned, int);
__int128 unsigned __tsan_atomic128_fetch_add(volatile void*, __int128 unsigned, int);
__int128 unsigned __tsan_atomic128_fetch_and(volatile void*, __int128 unsigned, int);
__int128 unsigned __tsan_atomic128_fetch_nand(volatile void*, __int128 unsigned, int);
__int128 unsigned __tsan_atomic128_fetch_or(volatile void*, __int128 unsigned, int);
__int128 unsigned __tsan_atomic128_fetch_sub(volatile void*, __int128 unsigned, int);
__int128 unsigned __tsan_atomic128_fetch_xor(volatile void*, __int128 unsigned, int);
__int128 unsigned __tsan_atomic128_load(const volatile void*, int);
void __tsan_atomic128_store(volatile void*, __int128 unsigned, int);

__int128 unsigned __builtin_bswap128(__int128 unsigned);
#endif /* defined(__SIZEOF_INT128__) */

__edg_bool_type__ __builtin___tsan_atomic16_compare_exchange_strong(volatile void*, void*, short unsigned int, int, int);
__edg_bool_type__ __builtin___tsan_atomic16_compare_exchange_weak(volatile void*, void*, short unsigned int, int, int);
short unsigned int __builtin___tsan_atomic16_exchange(volatile void*, short unsigned int, int);
short unsigned int __builtin___tsan_atomic16_fetch_add(volatile void*, short unsigned int, int);
short unsigned int __builtin___tsan_atomic16_fetch_and(volatile void*, short unsigned int, int);
short unsigned int __builtin___tsan_atomic16_fetch_nand(volatile void*, short unsigned int, int);
short unsigned int __builtin___tsan_atomic16_fetch_or(volatile void*, short unsigned int, int);
short unsigned int __builtin___tsan_atomic16_fetch_sub(volatile void*, short unsigned int, int);
short unsigned int __builtin___tsan_atomic16_fetch_xor(volatile void*, short unsigned int, int);
short unsigned int __builtin___tsan_atomic16_load(const volatile void*, int);
void __builtin___tsan_atomic16_store(volatile void*, short unsigned int, int);
__edg_bool_type__ __builtin___tsan_atomic32_compare_exchange_strong(volatile void*, void*, unsigned int, int, int);
__edg_bool_type__ __builtin___tsan_atomic32_compare_exchange_weak(volatile void*, void*, unsigned int, int, int);
unsigned int __builtin___tsan_atomic32_exchange(volatile void*, unsigned int, int);
unsigned int __builtin___tsan_atomic32_fetch_add(volatile void*, unsigned int, int);
unsigned int __builtin___tsan_atomic32_fetch_and(volatile void*, unsigned int, int);
unsigned int __builtin___tsan_atomic32_fetch_nand(volatile void*, unsigned int, int);
unsigned int __builtin___tsan_atomic32_fetch_or(volatile void*, unsigned int, int);
unsigned int __builtin___tsan_atomic32_fetch_sub(volatile void*, unsigned int, int);
unsigned int __builtin___tsan_atomic32_fetch_xor(volatile void*, unsigned int, int);
unsigned int __builtin___tsan_atomic32_load(const volatile void*, int);
void __builtin___tsan_atomic32_store(volatile void*, unsigned int, int);
__edg_bool_type__ __builtin___tsan_atomic64_compare_exchange_strong(volatile void*, void*, long unsigned int, int, int);
__edg_bool_type__ __builtin___tsan_atomic64_compare_exchange_weak(volatile void*, void*, long unsigned int, int, int);
long unsigned int __builtin___tsan_atomic64_exchange(volatile void*, long unsigned int, int);
long unsigned int __builtin___tsan_atomic64_fetch_add(volatile void*, long unsigned int, int);
long unsigned int __builtin___tsan_atomic64_fetch_and(volatile void*, long unsigned int, int);
long unsigned int __builtin___tsan_atomic64_fetch_nand(volatile void*, long unsigned int, int);
long unsigned int __builtin___tsan_atomic64_fetch_or(volatile void*, long unsigned int, int);
long unsigned int __builtin___tsan_atomic64_fetch_sub(volatile void*, long unsigned int, int);
long unsigned int __builtin___tsan_atomic64_fetch_xor(volatile void*, long unsigned int, int);
long unsigned int __builtin___tsan_atomic64_load(const volatile void*, int);
void __builtin___tsan_atomic64_store(volatile void*, long unsigned int, int);
__edg_bool_type__ __builtin___tsan_atomic8_compare_exchange_strong(volatile void*, void*, unsigned char, int, int);
__edg_bool_type__ __builtin___tsan_atomic8_compare_exchange_weak(volatile void*, void*, unsigned char, int, int);
unsigned char __builtin___tsan_atomic8_exchange(volatile void*, unsigned char, int);
unsigned char __builtin___tsan_atomic8_fetch_add(volatile void*, unsigned char, int);
unsigned char __builtin___tsan_atomic8_fetch_and(volatile void*, unsigned char, int);
unsigned char __builtin___tsan_atomic8_fetch_nand(volatile void*, unsigned char, int);
unsigned char __builtin___tsan_atomic8_fetch_or(volatile void*, unsigned char, int);
unsigned char __builtin___tsan_atomic8_fetch_sub(volatile void*, unsigned char, int);
unsigned char __builtin___tsan_atomic8_fetch_xor(volatile void*, unsigned char, int);
unsigned char __builtin___tsan_atomic8_load(const volatile void*, int);
void __builtin___tsan_atomic8_store(volatile void*, unsigned char, int);
void __builtin___tsan_atomic_signal_fence(int);
void __builtin___tsan_atomic_thread_fence(int);
void __builtin___tsan_func_entry(void*);
void __builtin___tsan_func_exit(void*);
void __builtin___tsan_read1(void*);
void __builtin___tsan_read16(void*);
void __builtin___tsan_read2(void*);
void __builtin___tsan_read4(void*);
void __builtin___tsan_read8(void*);
void __builtin___tsan_read_range(void*, long int);
#if __COVERITY_GCC_VERSION_AT_LEAST(5, 1)
void __builtin___tsan_vptr_update(void*, void*);
void __tsan_vptr_update(void*, void*);
#else
void __tsan_vptr_update(void*);
void __builtin___tsan_vptr_update(void*);
#endif
void __builtin___tsan_write1(void*);
void __builtin___tsan_write16(void*);
void __builtin___tsan_write2(void*);
void __builtin___tsan_write4(void*);
void __builtin___tsan_write8(void*);
void __builtin___tsan_write_range(void*, long int);
void __builtin___ubsan_handle_add_overflow(void*, void*, void*);
void __builtin___ubsan_handle_add_overflow_abort(void*, void*, void*);
void __builtin___ubsan_handle_builtin_unreachable(void*);
void __builtin___ubsan_handle_divrem_overflow(void*, void*, void*);
void __builtin___ubsan_handle_divrem_overflow_abort(void*, void*, void*);
void __builtin___ubsan_handle_dynamic_type_cache_miss(void*, void*, void*);
void __builtin___ubsan_handle_dynamic_type_cache_miss_abort(void*, void*, void*);
void __builtin___ubsan_handle_float_cast_overflow(void*, void*);
void __builtin___ubsan_handle_float_cast_overflow_abort(void*, void*);
void __builtin___ubsan_handle_invalid_builtin(void*);
void __builtin___ubsan_handle_invalid_builtin_abort(void*);
void __builtin___ubsan_handle_load_invalid_value(void*, void*);
void __builtin___ubsan_handle_load_invalid_value_abort(void*, void*);
void __builtin___ubsan_handle_missing_return(void*);
void __builtin___ubsan_handle_mul_overflow(void*, void*, void*);
void __builtin___ubsan_handle_mul_overflow_abort(void*, void*, void*);
void __builtin___ubsan_handle_negate_overflow(void*, void*);
void __builtin___ubsan_handle_negate_overflow_abort(void*, void*);
#if __COVERITY_GCC_VERSION_AT_LEAST(6, 0)
void __builtin___ubsan_handle_nonnull_arg(void*);
void __builtin___ubsan_handle_nonnull_arg_abort(void*);
void __ubsan_handle_nonnull_arg(void*);
void __ubsan_handle_nonnull_arg_abort(void*);
#else
void __ubsan_handle_nonnull_arg(void*, int);
void __ubsan_handle_nonnull_arg_abort(void*, int);
void __builtin___ubsan_handle_nonnull_arg(void*, int);
void __builtin___ubsan_handle_nonnull_arg_abort(void*, int);
#endif
void __builtin___ubsan_handle_nonnull_return_v1(void*, void*);
void __builtin___ubsan_handle_nonnull_return_v1_abort(void*, void*);
void __builtin___ubsan_handle_out_of_bounds(void*, void*);
void __builtin___ubsan_handle_out_of_bounds_abort(void*, void*);
void __builtin___ubsan_handle_pointer_overflow(void*, void*, void*);
void __builtin___ubsan_handle_pointer_overflow_abort(void*, void*, void*);
void __builtin___ubsan_handle_shift_out_of_bounds(void*, void*, void*);
void __builtin___ubsan_handle_shift_out_of_bounds_abort(void*, void*, void*);
void __builtin___ubsan_handle_sub_overflow(void*, void*, void*);
void __builtin___ubsan_handle_sub_overflow_abort(void*, void*, void*);
void __builtin___ubsan_handle_type_mismatch_v1(void*, void*);
void __builtin___ubsan_handle_type_mismatch_v1_abort(void*, void*);
void __builtin___ubsan_handle_vla_bound_not_positive(void*, void*);
void __builtin___ubsan_handle_vla_bound_not_positive_abort(void*, void*);
void __sanitizer_cov_trace_cmp1(unsigned char, unsigned char);
void __sanitizer_cov_trace_cmp2(short unsigned int, short unsigned int);
void __sanitizer_cov_trace_cmp4(unsigned int, unsigned int);
void __sanitizer_cov_trace_cmp8(long unsigned int, long unsigned int);
void __sanitizer_cov_trace_cmpd(double, double);
void __sanitizer_cov_trace_cmpf(float, float);
void __sanitizer_cov_trace_const_cmp1(unsigned char, unsigned char);
void __sanitizer_cov_trace_const_cmp2(short unsigned int, short unsigned int);
void __sanitizer_cov_trace_const_cmp4(unsigned int, unsigned int);
void __sanitizer_cov_trace_const_cmp8(long unsigned int, long unsigned int);
void __sanitizer_cov_trace_switch(long unsigned int, void*);
void __sanitizer_ptr_cmp(void*, long int);
void __sanitizer_ptr_sub(void*, long int);
__edg_bool_type__ __tsan_atomic16_compare_exchange_strong(volatile void*, void*, short unsigned int, int, int);
__edg_bool_type__ __tsan_atomic16_compare_exchange_weak(volatile void*, void*, short unsigned int, int, int);
short unsigned int __tsan_atomic16_exchange(volatile void*, short unsigned int, int);
short unsigned int __tsan_atomic16_fetch_add(volatile void*, short unsigned int, int);
short unsigned int __tsan_atomic16_fetch_and(volatile void*, short unsigned int, int);
short unsigned int __tsan_atomic16_fetch_nand(volatile void*, short unsigned int, int);
short unsigned int __tsan_atomic16_fetch_or(volatile void*, short unsigned int, int);
short unsigned int __tsan_atomic16_fetch_sub(volatile void*, short unsigned int, int);
short unsigned int __tsan_atomic16_fetch_xor(volatile void*, short unsigned int, int);
short unsigned int __tsan_atomic16_load(const volatile void*, int);
void __tsan_atomic16_store(volatile void*, short unsigned int, int);
__edg_bool_type__ __tsan_atomic32_compare_exchange_strong(volatile void*, void*, unsigned int, int, int);
__edg_bool_type__ __tsan_atomic32_compare_exchange_weak(volatile void*, void*, unsigned int, int, int);
unsigned int __tsan_atomic32_exchange(volatile void*, unsigned int, int);
unsigned int __tsan_atomic32_fetch_add(volatile void*, unsigned int, int);
unsigned int __tsan_atomic32_fetch_and(volatile void*, unsigned int, int);
unsigned int __tsan_atomic32_fetch_nand(volatile void*, unsigned int, int);
unsigned int __tsan_atomic32_fetch_or(volatile void*, unsigned int, int);
unsigned int __tsan_atomic32_fetch_sub(volatile void*, unsigned int, int);
unsigned int __tsan_atomic32_fetch_xor(volatile void*, unsigned int, int);
unsigned int __tsan_atomic32_load(const volatile void*, int);
void __tsan_atomic32_store(volatile void*, unsigned int, int);
__edg_bool_type__ __tsan_atomic64_compare_exchange_strong(volatile void*, void*, long unsigned int, int, int);
__edg_bool_type__ __tsan_atomic64_compare_exchange_weak(volatile void*, void*, long unsigned int, int, int);
long unsigned int __tsan_atomic64_exchange(volatile void*, long unsigned int, int);
long unsigned int __tsan_atomic64_fetch_add(volatile void*, long unsigned int, int);
long unsigned int __tsan_atomic64_fetch_and(volatile void*, long unsigned int, int);
long unsigned int __tsan_atomic64_fetch_nand(volatile void*, long unsigned int, int);
long unsigned int __tsan_atomic64_fetch_or(volatile void*, long unsigned int, int);
long unsigned int __tsan_atomic64_fetch_sub(volatile void*, long unsigned int, int);
long unsigned int __tsan_atomic64_fetch_xor(volatile void*, long unsigned int, int);
long unsigned int __tsan_atomic64_load(const volatile void*, int);
void __tsan_atomic64_store(volatile void*, long unsigned int, int);
__edg_bool_type__ __tsan_atomic8_compare_exchange_strong(volatile void*, void*, unsigned char, int, int);
__edg_bool_type__ __tsan_atomic8_compare_exchange_weak(volatile void*, void*, unsigned char, int, int);
unsigned char __tsan_atomic8_exchange(volatile void*, unsigned char, int);
unsigned char __tsan_atomic8_fetch_add(volatile void*, unsigned char, int);
unsigned char __tsan_atomic8_fetch_and(volatile void*, unsigned char, int);
unsigned char __tsan_atomic8_fetch_nand(volatile void*, unsigned char, int);
unsigned char __tsan_atomic8_fetch_or(volatile void*, unsigned char, int);
unsigned char __tsan_atomic8_fetch_sub(volatile void*, unsigned char, int);
unsigned char __tsan_atomic8_fetch_xor(volatile void*, unsigned char, int);
unsigned char __tsan_atomic8_load(const volatile void*, int);
void __tsan_atomic8_store(volatile void*, unsigned char, int);
void __tsan_atomic_signal_fence(int);
void __tsan_atomic_thread_fence(int);
void __tsan_func_entry(void*);
void __tsan_func_exit(void*);
void __tsan_read1(void*);
void __tsan_read16(void*);
void __tsan_read2(void*);
void __tsan_read4(void*);
void __tsan_read8(void*);
void __tsan_read_range(void*, long int);
void __tsan_write1(void*);
void __tsan_write16(void*);
void __tsan_write2(void*);
void __tsan_write4(void*);
void __tsan_write8(void*);
void __tsan_write_range(void*, long int);
void __ubsan_handle_add_overflow(void*, void*, void*);
void __ubsan_handle_add_overflow_abort(void*, void*, void*);
void __ubsan_handle_builtin_unreachable(void*);
void __ubsan_handle_divrem_overflow(void*, void*, void*);
void __ubsan_handle_divrem_overflow_abort(void*, void*, void*);
void __ubsan_handle_dynamic_type_cache_miss(void*, void*, void*);
void __ubsan_handle_dynamic_type_cache_miss_abort(void*, void*, void*);
void __ubsan_handle_float_cast_overflow(void*, void*);
void __ubsan_handle_float_cast_overflow_abort(void*, void*);
void __ubsan_handle_invalid_builtin(void*);
void __ubsan_handle_invalid_builtin_abort(void*);
void __ubsan_handle_load_invalid_value(void*, void*);
void __ubsan_handle_load_invalid_value_abort(void*, void*);
void __ubsan_handle_missing_return(void*);
void __ubsan_handle_mul_overflow(void*, void*, void*);
void __ubsan_handle_mul_overflow_abort(void*, void*, void*);
void __ubsan_handle_negate_overflow(void*, void*);
void __ubsan_handle_negate_overflow_abort(void*, void*);
void __ubsan_handle_nonnull_return_v1(void*, void*);
void __ubsan_handle_nonnull_return_v1_abort(void*, void*);
void __ubsan_handle_out_of_bounds(void*, void*);
void __ubsan_handle_out_of_bounds_abort(void*, void*);
void __ubsan_handle_pointer_overflow(void*, void*, void*);
void __ubsan_handle_pointer_overflow_abort(void*, void*, void*);
void __ubsan_handle_shift_out_of_bounds(void*, void*, void*);
void __ubsan_handle_shift_out_of_bounds_abort(void*, void*, void*);
void __ubsan_handle_sub_overflow(void*, void*, void*);
void __ubsan_handle_sub_overflow_abort(void*, void*, void*);
void __ubsan_handle_type_mismatch_v1(void*, void*);
void __ubsan_handle_type_mismatch_v1_abort(void*, void*);
void __ubsan_handle_vla_bound_not_positive(void*, void*);
void __ubsan_handle_vla_bound_not_positive_abort(void*, void*);

/* Copyright (c) 2023 Synopsys, Inc. All rights reserved worldwide. */

/*
 * These declarations are used to support Windows targets. They are not
 * supported intrinsically by cov-emit.
 */

#if __COVERITY_GCC_VERSION_AT_LEAST(4, 4)
#define __builtin_ms_va_list __builtin_va_list
#define __builtin_ms_va_copy __builtin_va_copy
#define __builtin_ms_va_start __builtin_va_start
#define __builtin_ms_va_end __builtin_va_end

#define __builtin_sysv_va_list __builtin_va_list
#define __builtin_sysv_va_copy __builtin_va_copy
#define __builtin_sysv_va_start __builtin_va_start
#define __builtin_sysv_va_end __builtin_va_end

#define __ms_va_copy(__d,__s) __builtin_ms_va_copy(__d,__s)
#define __ms_va_start(__v,__l) __builtin_ms_va_start(__v,__l)
#define __ms_va_arg(__v,__l)    __builtin_va_arg(__v,__l)
#define __ms_va_end(__v) __builtin_ms_va_end(__v)

#define __sysv_va_copy(__d,__s) __builtin_sysv_va_copy(__d,__s)
#define __sysv_va_start(__v,__l) __builtin_sysv_va_start(__v,__l)
#define __sysv_va_arg(__v,__l)  __builtin_va_arg(__v,__l)
#define __sysv_va_end(__v) __builtin_sysv_va_end(__v)
#endif /* GCC >= 4.4 */

#pragma builtin end
#endif /* COVERITY_COMPAT_4a622ee35a6c29d94c1d6dbcfdfbe3b3 */
#endif /* __COVERITY__ */
