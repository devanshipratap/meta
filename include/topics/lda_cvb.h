/**
 * @file lda_cvb.h
 *
 * All files in META are released under the MIT license. For more details,
 * consult the file LICENSE in the root of the project.
 */

#ifndef _DST_TOPICS_LDA_CVB_H_
#define _DST_TOPICS_LDA_CVB_H_

#include "topics/lda_model.h"

namespace meta
{
namespace topics
{

/**
 * lda_cvb: An implementation of LDA that uses collapsed variational bayes
 * for inference. Specifically, it uses the CVB0 algorithm detailed in
 * Asuncion et. al.
 *
 * @see http://www.ics.uci.edu/~asuncion/pubs/UAI_09.pdf
 */
class lda_cvb : public lda_model
{
  public:
    /**
     * Constructs the lda model over the given documents, with the
     * given number of topics, and hyperparameters \f$\alpha\f$ and
     * \f$\beta\f$ for the priors on \f$\phi\f$ (topic distributions)
     * and \f$\theta\f$ (topic proportions), respectively.
     *
     * Assumes that the given vector of documents will live for as long
     * as or longer than the lda_cvb instance.
     *
     * @param idx The index containing the documents to model
     * @param num_topics The number of topics to infer
     * @param alpha The hyperparameter for the Dirichlet prior over
     *  \f$\phi\f$.
     * @param beta The hyperparameter for the Dirichlet prior over
     *  \f$\theta\f$.
     */
    lda_cvb(index::forward_index& idx, uint64_t num_topics, double alpha,
            double beta);

    /**
     * Destructor: virtual for potential subclassing.
     */
    virtual ~lda_cvb() = default;

    /**
     * Runs the variational inference algorithm for a maximum number of
     * iterations, or until the given convergence criterion is met. The
     * convergence criterion is determined as the maximum difference in
     * any of the variational parameters \f$\gamma_{dij}\f$ in a given
     * iteration.
     *
     * @param num_iters The maximum number of iterations to run the
     *  sampler for.
     * @param convergence The lowest maximum difference in any
     *  \f$\gamma_{dij}\f$ to be allowed before considering the
     *  inference to have converged.
     */
    void run(uint64_t num_iters, double convergence = 1e-3);

  protected:
    /**
     * Initializes the parameters randomly.
     */
    void initialize();

    /**
     * Performs one iteration of the inference algorithm.
     *
     * @param iter the current iteration number
     * @return The maximum change in any of the \f$\gamma_{dij}\f$s.
     */
    double perform_iteration(uint64_t iter);

    virtual double compute_term_topic_probability(term_id term,
                                                  topic_id topic) const
        override;

    virtual double compute_doc_topic_probability(doc_id doc,
                                                 topic_id topic) const override;

    /**
     * Means for (document, topic) assignment counts.
     */
    std::unordered_map<doc_id, std::unordered_map<topic_id, double>>
        doc_topic_mean_;

    /**
     * Means for (topic, term) assignment counts.
     */
    std::unordered_map<topic_id, std::unordered_map<term_id, double>>
        topic_term_mean_;

    /**
     * Means for topic assignments.
     */
    std::unordered_map<topic_id, double> topic_mean_;

    /**
     * Variational parameters \f$\gamma_{dij}\f$.
     */
    std::unordered_map<
        doc_id, std::unordered_map<
                    term_id, std::unordered_map<topic_id, double>>> gamma_;

    /**
     * Hyperparameter on \f$\theta\f$, the topic proportions.
     */
    double alpha_;

    /**
     * Hyperparameter on \f$\phi\f$, the topic distributions.
     */
    double beta_;
};
}
}

#endif