/*
 NativeSyntaxHighlighting.re
 */

module Core = Oni_Core;
module Model = Oni_Model;

type t =
  | TreeSitter(TreeSitterSyntaxHighlights.t)
  | None;

let default = None;

let canHandleScope = (scope: string) =>
  switch (scope) {
  | "source.json" => true
  | "source.c" => true
  | "source.cpp" => true
  | _ => false
  };

let create = (~theme, ~getTreeSitterScopeMapper, lines: array(string)) => {
  let ts =
    TreeSitterSyntaxHighlights.create(
      ~theme,
      ~getTreeSitterScopeMapper,
      lines,
    );
  TreeSitter(ts);
};

let update = (
  ~bufferUpdate: Core.Types.BufferUpdate.t,
  ~lines: array(string),
  v: t) => {
  switch (v) {
  | TreeSitter(ts) => 
    let newTs: TreeSitterSyntaxHighlights.t = 
    TreeSitterSyntaxHighlights.update(~bufferUpdate, ~lines, ts);
    TreeSitter(newTs);
  | _ => v
  }
};

let getTokensForLine = (v: t, line: int) => {
  switch (v) {
  | TreeSitter(ts) => TreeSitterSyntaxHighlights.getTokenColors(ts, line)
  | _ => []
  };
};
