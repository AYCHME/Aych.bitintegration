#include <memory>
#include "primitives/block.h"
#include "primitives/transaction.h"
#include "chain.h"

void blockHandle(const std::shared_ptr<const CBlock> &block, const CBlockIndex *pindex, const std::vector<CTransactionRef>& vtrx);
void trxHandle(const CTransactionRef & vtrx);