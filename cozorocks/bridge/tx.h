//
// Created by Ziyang Hu on 2022/7/3.
//

#ifndef COZOROCKS_TX_H
#define COZOROCKS_TX_H

#include "common.h"
#include "slice.h"
#include "status.h"
#include "iter.h"

struct TxBridge {
    OptimisticTransactionDB *odb;
    TransactionDB *tdb;
    unique_ptr<Transaction> tx;
    unique_ptr<WriteOptions> w_opts;
    unique_ptr<ReadOptions> r_opts;
    unique_ptr<OptimisticTransactionOptions> o_tx_opts;
    unique_ptr<TransactionOptions> p_tx_opts;
    vector<ColumnFamilyHandle *> cf_handles;

    explicit TxBridge(TransactionDB *tdb_, vector<ColumnFamilyHandle *> cf_handles_) :
            odb(nullptr),
            tdb(tdb_),
            tx(),
            w_opts(new WriteOptions),
            r_opts(new ReadOptions),
            o_tx_opts(nullptr),
            p_tx_opts(new TransactionOptions),
            cf_handles(cf_handles_) {
        r_opts->ignore_range_deletions = true;
    }

    inline WriteOptions &get_w_opts() {
        return *w_opts;
    }

//    inline ReadOptions &get_r_opts() {
//        return *r_opts;
//    }

    inline void verify_checksums(bool val) {
        r_opts->verify_checksums = val;
    }

    inline void fill_cache(bool val) {
        r_opts->fill_cache = val;
    }

    inline unique_ptr<IterBridge> iterator(size_t idx) const {
        auto cf = cf_handles[idx];
        return make_unique<IterBridge>(&*tx, cf);
    };

    inline void set_snapshot(bool val) {
        if (tx != nullptr) {
            if (val) {
                tx->SetSnapshot();
            }
        } else if (o_tx_opts != nullptr) {
            o_tx_opts->set_snapshot = val;
        } else if (p_tx_opts != nullptr) {
            p_tx_opts->set_snapshot = val;
        }
    }

    inline void clear_snapshot() {
        tx->ClearSnapshot();
    }

    [[nodiscard]] inline DB *get_db() const {
        if (tdb != nullptr) {
            return tdb;
        } else {
            return odb;
        }
    }

    void start();

    inline unique_ptr<PinnableSlice> get(RustBytes key, bool for_update, size_t idx, RocksDbStatus &status) const {
        Slice key_ = convert_slice(key);
        auto ret = make_unique<PinnableSlice>();
        auto cf = cf_handles[idx];
        if (for_update) {
            auto s = tx->GetForUpdate(*r_opts, cf, key_, &*ret);
            write_status(s, status);
        } else {
            auto s = tx->Get(*r_opts, cf, key_, &*ret);
            write_status(s, status);
        }
        return ret;
    }

    inline void exists(RustBytes key, bool for_update, size_t idx, RocksDbStatus &status) const {
        Slice key_ = convert_slice(key);
        auto cf = cf_handles[idx];
        auto ret = PinnableSlice();
        if (for_update) {
            auto s = tx->GetForUpdate(*r_opts, cf, key_, &ret);
            write_status(s, status);
        } else {
            auto s = tx->Get(*r_opts, cf, key_, &ret);
            write_status(s, status);
        }
    }

    inline void put(RustBytes key, RustBytes val, size_t idx, RocksDbStatus &status) {
        auto cf = cf_handles[idx];
        write_status(tx->Put(cf, convert_slice(key), convert_slice(val)), status);
    }

    inline void del(RustBytes key, size_t idx, RocksDbStatus &status) {
        auto cf = cf_handles[idx];
        write_status(tx->Delete(cf, convert_slice(key)), status);
    }

    inline void commit(RocksDbStatus &status) {
        write_status(tx->Commit(), status);
    }

    inline void rollback(RocksDbStatus &status) {
        write_status(tx->Rollback(), status);
    }

    inline void rollback_to_savepoint(RocksDbStatus &status) {
        write_status(tx->RollbackToSavePoint(), status);
    }

    inline void pop_savepoint(RocksDbStatus &status) {
        write_status(tx->PopSavePoint(), status);
    }

    inline void set_savepoint() {
        tx->SetSavePoint();
    }
};

#endif //COZOROCKS_TX_H
