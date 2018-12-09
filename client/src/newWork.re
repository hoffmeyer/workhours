open Types;

type state = work;

type action =
  | ChangeStartDate(Js.Date.t)
  | ChangeStartTime(Js.Date.t)
  | ChangeDuration(float)
  | ChangeLunch(float)
  | SaveWork;

let component = ReasonReact.reducerComponent("NewWork");

let str = ReasonReact.string;

let createForm = (send, work) =>
  <div>
    <form id="registerHours">
      <label htmlFor="startDate"> {"Start date" |> str} </label>
      <DateInput
        id="startDate"
        value={work.start}
        onChange={date => send(ChangeStartDate(date))}
      />
      <label htmlFor="startTime"> {"Start time" |> str} </label>
      <TimeInput
        id="startTime"
        value={work.start}
        onChange={date => send(ChangeStartTime(date))}
      />
      <label htmlFor="duration"> {"Duration" |> str} </label>
      <input
        id="duration"
        value={work.duration |> Js.Float.toString}
        onChange={
          event =>
            send(ChangeDuration(Js.Float.fromString(ReactEvent.Form.target(event)##value)))
        }
        type_="number"
        min=0
        step=0.05
      />
      <label htmlFor="lunch"> {"Lunch" |> str} </label>
      <input
        id="lunch"
        value={work.lunch |> Js.Float.toString}
        onChange={
          event => send(ChangeLunch(Js.Float.fromString(ReactEvent.Form.target(event)##value)))
        }
        type_="number"
        min=0
        step=0.05
      />
      <button type_="button" onClick={_event => send(SaveWork)}> {"Add" |> str} </button>
    </form>
  </div>;

let make = (~handleAction: Types.action => unit, _children) => {
  ...component,
  initialState: _state => {
    id: None,
    start: Js.Date.make(),
    duration: 0.,
    lunch: 0.,
    userid: None,
  },
  reducer: (action, state) =>
    switch (action) {
    | ChangeStartDate(date) => ReasonReact.Update({...state, start: date})
    | ChangeStartTime(date) => ReasonReact.Update({...state, start: date})
    | ChangeDuration(duration) => ReasonReact.Update({...state, duration})
    | ChangeLunch(lunch) => ReasonReact.Update({...state, lunch})
    | SaveWork =>
      ReasonReact.UpdateWithSideEffects(
        state,
        (
          _self =>
            Js.Promise.(
              Work.save(state)
              |> then_(work =>
                   handleAction(Types.WorkAdd(work))
                   |> () => ReasonReact.Router.push("/")
                   |> resolve
                 )
              |> catch(err => {
                   Js.log2("Error starting work: ", err);
                   resolve();
                 })
              |> ignore
            )
        ),
      )
    },
  render: self =>
    <div>
      <h1> {ReasonReact.string("New registration")} </h1>
      <div> {createForm(self.send, self.state)} </div>
    </div>,
};