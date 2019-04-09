#include "../primitives/block.h"
#include "../primitives/transaction.h"
#include "../chain.h"
#include <string>

std::string blockInfo(const CBlock &oneBlock, const CBlockIndex &blockindex);
std::string transactionInfo(const CTransactionRef &vtx);