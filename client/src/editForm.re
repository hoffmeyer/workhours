open Types;

type action =
  | ChangeStartDate(Js.Date.t)
  | ChangeStartTime(Js.Date.t)
  | ChangeDuration(float)
  | ChangeLunch(float);

let str = ReasonReact.string;

let component = ReasonReact.reducerComponent("EditForm");

let make = (~work: work, ~submitAction: Types.work => unit, _children) => {
  ...component,
  initialState: () => work,
  reducer: (action: action, state: work) =>
    switch (action) {
    | ChangeStartDate(date) => ReasonReact.Update({...state, start: date})
    | ChangeStartTime(date) => ReasonReact.Update({...state, start: date})
    | ChangeDuration(duration) => ReasonReact.Update({...state, duration})
    | ChangeLunch(lunch) => ReasonReact.Update({...state, lunch})
    },
    render: self => <div>
    <form id="registerHours">
      <label htmlFor="startDate"> {"Start date" |> str} </label>
      <DateInput
        id="startDate"
        value={self.state.start}
        onChange={date => self.send(ChangeStartDate(date))}
      />
      <label htmlFor="startTime"> {"Start time" |> str} </label>
      <TimeInput
        id="startTime"
        value={self.state.start}
        onChange={date => self.send(ChangeStartTime(date))}
      />
      <label htmlFor="duration"> {"Duration" |> str} </label>
      <input
        id="duration"
        value={self.state.duration |> Js.Float.toString}
        onChange={
          event =>
            self.send(ChangeDuration(Js.Float.fromString(ReactEvent.Form.target(event)##value)))
        }
        type_="number"
        min=0
        step=0.05
      />
      <label htmlFor="lunch"> {"Lunch" |> str} </label>
      <input
        id="lunch"
        value={self.state.lunch |> Js.Float.toString}
        onChange={
          event => self.send(ChangeLunch(Js.Float.fromString(ReactEvent.Form.target(event)##value)))
        }
        type_="number"
        min=0
        step=0.05
      />
      <button type_="button" onClick={_event => submitAction(self.state)}> {"Add" |> str} </button>
    </form>
  </div>
};