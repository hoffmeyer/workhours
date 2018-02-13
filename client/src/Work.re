let component = ReasonReact.statelessComponent("Work");

let make = (_children) => {
    ...component,
    render: (_self) =>
        <div>
            <h1>(ReasonReact.stringToElement("Work page"))</h1>
        </div>
};