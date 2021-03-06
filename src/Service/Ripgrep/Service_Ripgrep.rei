open Oni_Core;

module Sub: {
  type findInFilesMsg =
    | GotMatches(list(Ripgrep.Match.t))
    | Completed
    | Error(string);

  let findInFiles:
    (
      ~uniqueId: string,
      ~exclude: list(string),
      ~directory: string,
      ~query: string,
      ~setup: Setup.t,
      findInFilesMsg => 'msg
    ) =>
    Isolinear.Sub.t('msg);
};
