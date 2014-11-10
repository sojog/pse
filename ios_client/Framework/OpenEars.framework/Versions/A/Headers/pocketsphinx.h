/* -*- c-basic-offset:4; indent-tabs-mode: nil -*- */
/* ====================================================================
 * Copyright (c) 1999-2008 Carnegie Mellon University.  All rights
 * reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer. 
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 *
 * THIS SOFTWARE IS PROVIDED BY CARNEGIE MELLON UNIVERSITY ``AS IS'' AND 
 * ANY EXPRESSED OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL CARNEGIE MELLON UNIVERSITY
 * NOR ITS EMPLOYEES BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY 
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * ====================================================================
 *
 */
/**
 * @file pocketsphinx.h Main header file for the PocketSphinx decoder.
 */

#ifndef __POCKETSPHINX_H__
#define __POCKETSPHINX_H__

#ifdef __cplusplus
extern "C" {
#endif
#if 0
}
#endif

#import <OpenEars/OpenEarsStaticAnalysisToggle.h>

#ifdef SHOW64BITCOMPLAINTS
#else
#if __LP64__ // This is only significant for 64-bit compilations -- prefer to keep it limited.
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wshorten-64-to-32" // We are turning off warnings about 64 bit shortening because it isn't harming behavior, but it's reversible for troubleshooting. 
#endif
#endif

/* System headers we need. */
#include <stdio.h>

/* SphinxBase headers we need. */
#include "cmd_ln.h"
#include "logmath.h"
#include "fe.h"
#include "feat.h"
#include "ngram_model.h"

/* PocketSphinx headers (not many of them!) */
#include "pocketsphinx_export.h"
#include "cmdln_macro.h"
#include "ps_lattice.h"
#include "ps_mllr.h"
#include "fsg_set.h"

/**
 * PocketSphinx speech recognizer object.
 */
typedef struct ps_decoder_s ps_decoder_t;

/**
 * PocketSphinx N-best hypothesis iterator object.
 */
typedef struct ps_astar_s ps_nbest_t;

/**
 * PocketSphinx segmentation iterator object.
 */
typedef struct ps_seg_s ps_seg_t;

/**
 * Initialize the decoder from a configuration object.
 *
 * @note The decoder retains ownership of the pointer
 * <code>config</code>, so you must not attempt to free it manually.
 * If you wish to reuse it elsewhere, call cmd_ln_retain() on it.
 *
 * @param config a command-line structure, as created by
 * cmd_ln_parse_r() or cmd_ln_parse_file_r().
 */
POCKETSPHINX_EXPORT
ps_decoder_t *ps_init(cmd_ln_t *config);

/**
 * Reinitialize the decoder with updated configuration.
 *
 * This function allows you to switch the acoustic model, dictionary,
 * or other configuration without creating an entirely new decoding
 * object.
 *
 * @note The decoder retains ownership of the pointer
 * <code>config</code>, so you must not attempt to free it manually.
 * If you wish to reuse it elsewhere, call cmd_ln_retain() on it.
 *
 * @param ps Decoder.
 * @param config An optional new configuration to use.  If this is
 *               NULL, the previous configuration will be reloaded,
 *               with any changes applied.
 * @return 0 for success, <0 for failure.
 */
POCKETSPHINX_EXPORT
int ps_reinit(ps_decoder_t *ps, cmd_ln_t *config);

/**
 * Returns the argument definitions used in ps_init().
 *
 * This is here to avoid exporting global data, which is problematic
 * on Win32 and Symbian (and possibly other platforms).
 */
POCKETSPHINX_EXPORT
arg_t const *ps_args(void);

/**
 * Retain a pointer to the decoder.
 *
 * This increments the reference count on the decoder, allowing it to
 * be shared between multiple parent objects.  In general you will not
 * need to use this function, ever.  It is mainly here for the
 * convenience of scripting language bindings.
 *
 * @return pointer to retained decoder.
 */
POCKETSPHINX_EXPORT
ps_decoder_t *ps_retain(ps_decoder_t *ps);

/**
 * Finalize the decoder.
 *
 * This releases all resources associated with the decoder, including
 * any language models or grammars which have been added to it, and
 * the initial configuration object passed to ps_init().
 *
 * @param ps Decoder to be freed.
 * @return New reference count (0 if freed).
 */
POCKETSPHINX_EXPORT
int ps_free(ps_decoder_t *ps);

/**
 * Get the configuration object for this decoder.
 *
 * @return The configuration object for this decoder.  The decoder
 *         retains ownership of this pointer, so you should not
 *         attempt to free it manually.  Use cmd_ln_retain() if you
 *         wish to reuse it elsewhere.
 */
POCKETSPHINX_EXPORT
cmd_ln_t *ps_get_config(ps_decoder_t *ps);

/**
 * Get the log-math computation object for this decoder.
 *
 * @return The log-math object for this decoder.  The decoder retains
 *         ownership of this pointer, so you should not attempt to
 *         free it manually.  Use logmath_retain() if you wish to
 *         reuse it elsewhere.
 */
POCKETSPHINX_EXPORT
logmath_t *ps_get_logmath(ps_decoder_t *ps);

/**
 * Get the feature extraction object for this decoder.
 *
 * @return The feature extraction object for this decoder.  The
 *         decoder retains ownership of this pointer, so you should
 *         not attempt to free it manually.  Use fe_retain() if you
 *         wish to reuse it elsewhere.
 */
POCKETSPHINX_EXPORT
fe_t *ps_get_fe(ps_decoder_t *ps);

/**
 * Get the dynamic feature computation object for this decoder.
 *
 * @return The dynamic feature computation object for this decoder.  The
 *         decoder retains ownership of this pointer, so you should
 *         not attempt to free it manually.  Use feat_retain() if you
 *         wish to reuse it elsewhere.
 */
POCKETSPHINX_EXPORT
feat_t *ps_get_feat(ps_decoder_t *ps);

/**
 * Adapt current acoustic model using a linear transform.
 *
 * @param mllr The new transform to use, or NULL to update the existing
 *              transform.  The decoder retains ownership of this pointer,
 *              so you should not attempt to free it manually.  Use
 *              ps_mllr_retain() if you wish to reuse it
 *              elsewhere.
 * @return The updated transform object for this decoder, or
 *         NULL on failure.
 */
POCKETSPHINX_EXPORT
ps_mllr_t *ps_update_mllr(ps_decoder_t *ps, ps_mllr_t *mllr);

/**
 * Get the language model set object for this decoder.
 *
 * If N-Gram decoding is not enabled, this will return NULL.  You will
 * need to enable it using ps_update_lmset().
 *
 * @return The language model set object for this decoder.  The
 *         decoder retains ownership of this pointer, so you should
 *         not attempt to free it manually.  Use ngram_model_retain()
 *         if you wish to reuse it elsewhere.
 */
POCKETSPHINX_EXPORT
ngram_model_t *ps_get_lmset(ps_decoder_t *ps);

/**
 * Update the language model set object for this decoder.
 *
 * This function does several things.  Most importantly, it enables
 * N-Gram decoding if not currently enabled.  It also updates internal
 * data structures to reflect any changes made to the language model
 * set (e.g. switching language models, adding words, etc).
 *
 * @param lmset The new lmset to use, or NULL to update the existing
 *              lmset.  The decoder retains ownership of this pointer,
 *              so you should not attempt to free it manually.  Use
 *              ngram_model_retain() if you wish to reuse it
 *              elsewhere.
 * @return The updated language model set object for this decoder, or
 *         NULL on failure.
 */
POCKETSPHINX_EXPORT
ngram_model_t *ps_update_lmset(ps_decoder_t *ps, ngram_model_t *lmset);

/**
 * Get the finite-state grammar set object for this decoder.
 *
 * If FSG decoding is not enabled, this returns NULL.  Call
 * ps_update_fsgset() to enable it.
 *
 * @return The current FSG set object for this decoder, or
 *         NULL if none is available.
 */
POCKETSPHINX_EXPORT
fsg_set_t *ps_get_fsgset(ps_decoder_t *ps);

/**
 * Update the finite-state grammar set object for this decoder.
 *
 * This function does several things.  Most importantly, it enables
 * FSG decoding if not currently enabled.  It also updates internal
 * data structures to reflect any changes made to the FSG set.
 *
 * @return The current FSG set object for this decoder, or
 *         NULL on failure.
 */
POCKETSPHINX_EXPORT
fsg_set_t *ps_update_fsgset(ps_decoder_t *ps);

/**
 * Reload the pronunciation dictionary from a file.
 *
 * This function replaces the current pronunciation dictionary with
 * the one stored in dictfile.  This also causes the active search
 * module(s) to be reinitialized, in the same manner as calling
 * ps_add_word() with update=TRUE.
 *
 * @param dictfile Path to dictionary file to load.
 * @param fdictfile Path to filler dictionary to load, or NULL to keep
 *                  the existing filler dictionary.
 * @param format Format of the dictionary file, or NULL to determine
 *               automatically (currently unused,should be NULL)
 */
POCKETSPHINX_EXPORT
int ps_load_dict(ps_decoder_t *ps, char const *dictfile,
                 char const *fdictfile, char const *format);

/**
 * Dump the current pronunciation dictionary to a file.
 *
 * This function dumps the current pronunciation dictionary to a tex
 *
 * @param dictfile Path to file where dictionary will be written.
 * @param format Format of the dictionary file, or NULL for the
 *               default (text) format (currently unused, should be NULL)
 */
POCKETSPHINX_EXPORT
int ps_save_dict(ps_decoder_t *ps, char const *dictfile, char const *format);

/**
 * Add a word to the pronunciation dictionary.
 *
 * This function adds a word to the pronunciation dictionary and the
 * current language model (but, obviously, not to the current FSG if
 * FSG mode is enabled).  If the word is already present in one or the
 * other, it does whatever is necessary to ensure that the word can be
 * recognized.
 *
 * @param word Word string to add.
 * @param phones Whitespace-separated list of phoneme strings
 *               describing pronunciation of <code>word</code>.
 * @param update If TRUE, update the search module (whichever one is
 *               currently active) to recognize the newly added word.
 *               If adding multiple words, it is more efficient to
 *               pass FALSE here in all but the last word.
 * @return The internal ID (>= 0) of the newly added word, or <0 on
 *         failure.
 */
POCKETSPHINX_EXPORT
int ps_add_word(ps_decoder_t *ps,
                char const *word,
                char const *phones,
                int update);

/**
 * Decode a raw audio stream.
 *
 * No headers are recognized in this files.  The configuration
 * parameters <tt>-samprate</tt> and <tt>-input_endian</tt> are used
 * to determine the sampling rate and endianness of the stream,
 * respectively.  Audio is always assumed to be 16-bit signed PCM.
 *
 * @param ps Decoder.
 * @param rawfh Previously opened file stream.
 * @param uttid Utterance ID (or NULL to generate automatically).
 * @param maxsamps Maximum number of samples to read from rawfh, or -1
 *                 to read until end-of-file.
 * @return Number of samples of audio.
 */
POCKETSPHINX_EXPORT
int ps_decode_raw(ps_decoder_t *ps, FILE *rawfh,
                  char const *uttid, long maxsamps);

/**
 * Decode a senone score dump file.
 *
 * @param ps Decoder
 * @param fh Previously opened file handle positioned at start of file.
 * @param uttid Utterance ID (or NULL to generate automatically).
 * @return Number of frames read.
 */
POCKETSPHINX_EXPORT
int ps_decode_senscr(ps_decoder_t *ps, FILE *senfh,
                     char const *uttid);

/**
 * Start utterance processing.
 *
 * This function should be called before any utterance data is passed
 * to the decoder.  It marks the start of a new utterance and
 * reinitializes internal data structures.
 *
 * @param ps Decoder to be started.
 * @param uttid String uniquely identifying this utterance.  If NULL,
 *              one will be created.
 * @return 0 for success, <0 on error.
 */
POCKETSPHINX_EXPORT
int ps_start_utt(ps_decoder_t *ps, char const *uttid);

/**
 * Get current utterance ID.
 *
 * @param ps Decoder to query.
 * @return Read-only string of the current utterance ID.  This is
 * valid only until the beginning of the next utterance.
 */
POCKETSPHINX_EXPORT
char const *ps_get_uttid(ps_decoder_t *ps);
/**
 * Decode raw audio data.
 *
 * @param ps Decoder.
 * @param no_search If non-zero, perform feature extraction but don't
 *                  do any recognition yet.  This may be necessary if
 *                  your processor has trouble doing recognition in
 *                  real-time.
 * @param full_utt If non-zero, this block of data is a full utterance
 *                 worth of data.  This may allow the recognizer to
 *                 produce more accurate results.
 * @return Number of frames of data searched, or <0 for error.
 */
POCKETSPHINX_EXPORT
int ps_process_raw(ps_decoder_t *ps,
                   int16 const *data,
                   size_t n_samples,
                   int no_search,
                   int full_utt);

/**
 * Decode acoustic feature data.
 *
 * @param ps Decoder.
 * @param no_search If non-zero, perform feature extraction but don't
 *                  do any recognition yet.  This may be necessary if
 *                  your processor has trouble doing recognition in
 *                  real-time.
 * @param full_utt If non-zero, this block of data is a full utterance
 *                 worth of data.  This may allow the recognizer to
 *                 produce more accurate results.
 * @return Number of frames of data searched, or <0 for error.
 */
POCKETSPHINX_EXPORT
int ps_process_cep(ps_decoder_t *ps,
                   mfcc_t **data,
                   int n_frames,
                   int no_search,
                   int full_utt);

/**
 * Get the number of frames of data searched.
 *
 * Note that there is a delay between this and the number of frames of
 * audio which have been input to the system.  This is due to the fact
 * that acoustic features are computed using a sliding window of
 * audio, and dynamic features are computed over a sliding window of
 * acoustic features.
 *
 * @param ps Decoder.
 * @return Number of frames of speech data which have been recognized
 * so far.
 */
POCKETSPHINX_EXPORT
int ps_get_n_frames(ps_decoder_t *ps);

/**
 * End utterance processing.
 *
 * @param ps Decoder.
 * @return 0 for success, <0 on error
 */
POCKETSPHINX_EXPORT
int ps_end_utt(ps_decoder_t *ps);

/**
 * Get hypothesis string and path score.
 *
 * @param ps Decoder.
 * @param out_best_score Output: path score corresponding to returned string.
 * @param out_uttid Output: utterance ID for this utterance.
 * @return String containing best hypothesis at this point in
 *         decoding.  NULL if no hypothesis is available.
 */
POCKETSPHINX_EXPORT
char const *ps_get_hyp(ps_decoder_t *ps, int32 *out_best_score,
                       char const **out_uttid);

/**
 * Get hypothesis string and final flag.
 *
 * @param ps Decoder.
 * @param out_is_best_score Output: if hypothesis is reached final state in the grammar.
 * @return String containing best hypothesis at this point in
 *         decoding.  NULL if no hypothesis is available.
 */
POCKETSPHINX_EXPORT
char const *ps_get_hyp_final(ps_decoder_t *ps, int32 *out_is_final);

/**
 * Get posterior probability.
 *
 * @note Unless the -bestpath option is enabled, this function will
 * always return zero (corresponding to a posterior probability of
 * 1.0).  Even if -bestpath is enabled, it will also return zero when
 * called on a partial result.  Ongoing research into effective
 * confidence annotation for partial hypotheses may result in these
 * restrictions being lifted in future versions.
 *
 * @param ps Decoder.
 * @param out_uttid Output: utterance ID for this utterance.
 * @return Posterior probability of the best hypothesis.
 */
POCKETSPHINX_EXPORT
int32 ps_get_prob(ps_decoder_t *ps, char const **out_uttid);

/**
 * Get word lattice.
 *
 * There isn't much you can do with this so far, a public API will
 * appear in the future.
 *
 * @param ps Decoder.
 * @return Word lattice object containing all hypotheses so far.  NULL
 *         if no hypotheses are available.  This pointer is owned by
 *         the decoder and you should not attempt to free it manually.
 *         It is only valid until the next utterance, unless you use
 *         ps_lattice_retain() to retain it.
 */
POCKETSPHINX_EXPORT
ps_lattice_t *ps_get_lattice(ps_decoder_t *ps);

/**
 * Get an iterator over the word segmentation for the best hypothesis.
 *
 * @param ps Decoder.
 * @param out_best_score Output: path score corresponding to hypothesis.
 * @return Iterator over the best hypothesis at this point in
 *         decoding.  NULL if no hypothesis is available.
 */
POCKETSPHINX_EXPORT
ps_seg_t *ps_seg_iter(ps_decoder_t *ps, int32 *out_best_score);

/**
 * Get the next segment in a word segmentation.
 *
 * @param seg Segment iterator.
 * @return Updated iterator with the next segment.  NULL at end of
 *         utterance (the iterator will be freed in this case).
 */
POCKETSPHINX_EXPORT
ps_seg_t *ps_seg_next(ps_seg_t *seg);

/**
 * Get word string from a segmentation iterator.
 *
 * @param seg Segment iterator.
 * @return Read-only string giving string name of this segment.  This
 * is only valid until the next call to ps_seg_next().
 */
POCKETSPHINX_EXPORT
char const *ps_seg_word(ps_seg_t *seg);

/**
 * Get inclusive start and end frames from a segmentation iterator.
 *
 * @note These frame numbers are inclusive, i.e. the end frame refers
 * to the last frame in which the given word or other segment was
 * active.  Therefore, the actual duration is *out_ef - *out_sf + 1.
 *
 * @param seg Segment iterator.
 * @param out_sf Output: First frame index in segment.
 * @param out_sf Output: Last frame index in segment.
 */
POCKETSPHINX_EXPORT
void ps_seg_frames(ps_seg_t *seg, int *out_sf, int *out_ef);

/**
 * Get language, acoustic, and posterior probabilities from a
 * segmentation iterator.
 *
 * @note Unless the -bestpath option is enabled, this function will
 * always return zero (corresponding to a posterior probability of
 * 1.0).  Even if -bestpath is enabled, it will also return zero when
 * called on a partial result.  Ongoing research into effective
 * confidence annotation for partial hypotheses may result in these
 * restrictions being lifted in future versions.
 *
 * @param out_ascr Output: acoustic model score for this segment.
 * @param out_lscr Output: language model score for this segment.
 * @param out_lback Output: language model backoff mode for this
 *                  segment (i.e. the number of words used in
 *                  calculating lscr).  This field is, of course, only
 *                  meaningful for N-Gram models.
 * @return Log posterior probability of current segment.  Log is
 *         expressed in the log-base used in the decoder.  To convert
 *         to linear floating-point, use logmath_exp(ps_get_logmath(),
 *         pprob).
 */
POCKETSPHINX_EXPORT
int32 ps_seg_prob(ps_seg_t *seg, int32 *out_ascr, int32 *out_lscr, int32 *out_lback);

/**
 * Finish iterating over a word segmentation early, freeing resources.
 */
POCKETSPHINX_EXPORT
void ps_seg_free(ps_seg_t *seg);

/**
 * Get an iterator over the best hypotheses, optionally within a
 * selected region of the utterance. Iterator is empty now, it must
 * be advanced with ps_nbest_next first. The function may also
 * return a NULL which means that there is nohypothesis available for this
 * utterance.
 *
 * @param ps Decoder.
 * @param sf Start frame for N-best search (0 for whole utterance) 
 * @param ef End frame for N-best search (-1 for whole utterance) 
 * @param ctx1 First word of trigram context (NULL for whole utterance)
 * @param ctx2 First word of trigram context (NULL for whole utterance)
 * @return Iterator over N-best hypotheses or NULL if no hypothesis is available
 */
POCKETSPHINX_EXPORT
ps_nbest_t *ps_nbest(ps_decoder_t *ps, int sf, int ef,
                     char const *ctx1, char const *ctx2);

/**
 * Move an N-best list iterator forward.
 *
 * @param nbest N-best iterator.
 * @return Updated N-best iterator, or NULL if no more hypotheses are
 *         available (iterator is freed ni this case).
 */
POCKETSPHINX_EXPORT 
ps_nbest_t *ps_nbest_next(ps_nbest_t *nbest);

/**
 * Get the hypothesis string from an N-best list iterator.
 *
 * @param nbest N-best iterator.
 * @param out_score Output: Path score for this hypothesis.
 * @return String containing next best hypothesis.
 */
POCKETSPHINX_EXPORT
char const *ps_nbest_hyp(ps_nbest_t *nbest, int32 *out_score);

/**
 * Get the word segmentation from an N-best list iterator.
 *
 * @param nbest N-best iterator.
 * @param out_score Output: Path score for this hypothesis.
 * @return Iterator over the next best hypothesis.
 */
POCKETSPHINX_EXPORT
ps_seg_t *ps_nbest_seg(ps_nbest_t *nbest, int32 *out_score);

/**
 * Finish N-best search early, releasing resources.
 *
 * @param nbest N-best iterator.
 */
POCKETSPHINX_EXPORT
void ps_nbest_free(ps_nbest_t *nbest);

/**
 * Get performance information for the current utterance.
 *
 * @param ps Decoder.
 * @param out_nspeech Output: Number of seconds of speech.
 * @param out_ncpu    Output: Number of seconds of CPU time used.
 * @param out_nwall   Output: Number of seconds of wall time used.
 */
POCKETSPHINX_EXPORT
void ps_get_utt_time(ps_decoder_t *ps, double *out_nspeech,
                     double *out_ncpu, double *out_nwall);

/**
 * Get overall performance information.
 *
 * @param ps Decoder.
 * @param out_nspeech Output: Number of seconds of speech.
 * @param out_ncpu    Output: Number of seconds of CPU time used.
 * @param out_nwall   Output: Number of seconds of wall time used.
 */
POCKETSPHINX_EXPORT
void ps_get_all_time(ps_decoder_t *ps, double *out_nspeech,
                     double *out_ncpu, double *out_nwall);

/**
 * @mainpage PocketSphinx API Documentation
 * @author David Huggins-Daines <dhuggins@cs.cmu.edu>
 * @version 0.6
 * @date March, 2010
 *
 * @section intro_sec Introduction
 *
 * This is the API documentation for the PocketSphinx speech
 * recognition engine.  The main API calls are documented in
 * <pocketsphinx.h>.
 */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __POCKETSPHINX_H__ */