open Oni_Core;

[@deriving show({with_path: false})]
type metadata = {
  path: Fp.t(Fp.absolute),
  displayName: string,
  hash: int // hash of basename, so only comparable locally
};

[@deriving show({with_path: false})]
type t = Tree.t(metadata, metadata);

let file: Fp.t(Fp.absolute) => t;
let directory: (~isOpen: bool=?, Fp.t(Fp.absolute), ~children: list(t)) => t;

let getPath: t => Fp.t(Fp.absolute);
let displayName: t => string;

let findNodesByPath:
  (Fp.t(Fp.absolute), t) => [ | `Success(list(t)) | `Partial(t) | `Failed];
let findByPath: (Fp.t(Fp.absolute), t) => option(t);

let replace: (~replacement: t, t) => t;
let updateNodesInPath: (t => t, Fp.t(Fp.absolute), t) => t;
let toggleOpen: t => t;
let setOpen: t => t;

let equals: (t, t) => bool;

module Model: {
  type nonrec t = t;

  let children: t => list(t);
  let kind: t => [ | `Node([ | `Open | `Closed]) | `Leaf];
};
