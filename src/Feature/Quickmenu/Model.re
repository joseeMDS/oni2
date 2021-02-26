open Oni_Core;

[@deriving show]
type msg =
  | KeyPressed(string)
  | Pasted(string)
  | Input(Component_InputText.msg);

module Msg = {
  let keyPressed = key => KeyPressed(key);
  let pasted = key => Pasted(key);
};

type progress =
  | Loading
  | InProgress(float)
  | Complete;

type model('outmsg) = {menus: list(Schema.Instance.t('outmsg))};

let initial = {menus: []};

let isMenuOpen = ({menus}) => menus != [];

let show = (~menu, model) => {
  menus: [Schema.instantiate(menu), ...model.menus],
};

let current = model => {
  switch (model.menus) {
  | [] => None
  | [hd, ..._] => Some(hd)
  };
};

// UPDATE

type outmsg('action) =
  | Action('action)
  | Nothing;

let updateCurrentMenu = (f, model) => {
  let menus' =
    switch (model.menus) {
    | [] => []
    | [current, ...others] => [f(current), ...others]
    };
  {...model, menus: menus'};
};

let currentMenu = model => {
  switch (model.menus) {
  | [] => None
  | [current, ..._] => Some(current)
  };
};

let focus = (~index, model) => {
  prerr_endline("!! FOCUS");
  model;
};

let next = model => {
  updateCurrentMenu(Schema.Instance.next, model);
};

let prev = model => {
  updateCurrentMenu(Schema.Instance.previous, model);
};

let cancel = _model => {menus: []};

let select = model => {
  let eff =
    model
    |> currentMenu
    |> Utility.OptionEx.flatMap(menu => Schema.Instance.select(menu))
    |> Option.map(msg => {
         prerr_endline("CREATING VALUE");
         EffectEx.value(~name="Feature_Quickmenu.select", msg);
       })
    |> Option.value(~default=Isolinear.Effect.none);

  ({menus: []}, eff);
};

let update = (msg, model) => {
  Schema.Instance.(
    switch (msg) {
    | Pasted(text) => (
        model |> updateCurrentMenu(Schema.Instance.paste(~text)),
        Nothing,
      )

    | KeyPressed(key) => (
        model |> updateCurrentMenu(Schema.Instance.key(~key)),
        Nothing,
      )

    | Input(msg) => (
        model |> updateCurrentMenu(Schema.Instance.input(msg)),
        Nothing,
      )
    }
  );
};

let contextKeys = model => {
  WhenExpr.ContextKeys.(
    [
      Schema.bool("listFocus", isMenuOpen),
      Schema.bool("inQuickOpen", isMenuOpen),
    ]
    |> Schema.fromList
    |> fromSchema(model)
  );
};
