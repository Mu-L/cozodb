// use std::collections::BTreeMap;
//
// use anyhow::Result;
// use smartstring::{LazyCompact, SmartString};
//
// use crate::algo::AlgoImpl;
// use crate::data::expr::Expr;
// use crate::data::program::{MagicAlgoRuleArg, MagicSymbol};
// use crate::runtime::derived::DerivedRelStore;
// use crate::runtime::transact::SessionTx;
//
// pub(crate) struct ClusteringCoefficient;
//
// impl AlgoImpl for ClusteringCoefficient {
//     fn run(
//         &mut self,
//         tx: &SessionTx,
//         rels: &[MagicAlgoRuleArg],
//         opts: &BTreeMap<SmartString<LazyCompact>, Expr>,
//         stores: &BTreeMap<MagicSymbol, DerivedRelStore>,
//         out: &DerivedRelStore,
//     ) -> Result<()> {
//         todo!()
//     }
// }
//
// pub(crate) struct Triangles;
//
// impl AlgoImpl for Triangles {
//     fn run(
//         &mut self,
//         tx: &SessionTx,
//         rels: &[MagicAlgoRuleArg],
//         opts: &BTreeMap<SmartString<LazyCompact>, Expr>,
//         stores: &BTreeMap<MagicSymbol, DerivedRelStore>,
//         out: &DerivedRelStore,
//     ) -> Result<()> {
//         todo!()
//     }
// }