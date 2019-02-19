open Types;

let component = ReasonReact.statelessComponent("Home");

let make =
    (~workList: array(work), ~handleAction: Types.action => unit, _children) => {
  ...component,
  render: _self =>
    <div>
      <Balance />
      <CurrentWork workList handleAction />
      <WorkList workList handleAction />
      <button
        className="addBtn"
        onClick={_event =>
          ReasonReact.Router.push(Router.routeToString(Router.Edit(None)))
        }>
        {"+" |> ReasonReact.string}
      </button>
    </div>,
};