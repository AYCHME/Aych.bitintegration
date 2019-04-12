#include <univalue.h>
#include <tinyformat.h>
#include "rpc/blockchain.h"
#include "consensus/validation.h"
#include "core_io.h"
#include "serialize.h"
#include "version.h"
#include "info.hpp"

std::string blockInfo(const CBlock &oneBlock, const CBlockIndex &blockindex){
    UniValue result(UniValue::VOBJ);

    //block header
    result.pushKV("version", oneBlock.nVersion);
    result.pushKV("time", oneBlock.GetBlockTime());
    result.pushKV("nonce", (uint64_t)oneBlock.nNonce);
    result.pushKV("bits", strprintf("%08x", oneBlock.nBits));
    result.pushKV("merkleroot", oneBlock.hashMerkleRoot.GetHex());
    result.pushKV("preblock", oneBlock.hashPrevBlock.GetHex());

    //block index
    result.pushKV("hash", blockindex.GetBlockHash().GetHex());
    result.pushKV("height", blockindex.nHeight);
    result.pushKV("nTx", (uint64_t)blockindex.nTx);
    result.pushKV("difficulty", GetDifficulty(&blockindex));
    result.pushKV("size", (int)::GetSerializeSize(oneBlock, SER_NETWORK, PROTOCOL_VERSION));
    result.pushKV("weight", (int)::GetBlockWeight(oneBlock));
    result.pushKV("mediantime", (int64_t)blockindex.GetMedianTimePast());
    result.pushKV("chainwork", blockindex.nChainWork.GetHex());

    //transaction info
    UniValue txs(UniValue::VARR);
    for(const auto& tx : oneBlock.vtx)
    {
        UniValue objTx(UniValue::VOBJ);
        TxToUniv(*tx, uint256(), objTx);
        txs.push_back(objTx);
    }
    result.pushKV("txs", txs);

    //json data
    return result.write() + "\n";
}

std::string transactionInfo(const CTransactionRef &vtx){
    UniValue result(UniValue::VOBJ);
    TxToUniv(*vtx, uint256(), result);

     //json data
    return result.write() + "\n";
}
