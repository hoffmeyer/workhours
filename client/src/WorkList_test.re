open Jest;

open Types;

open Js.Date;

let createWorkOnDate = (id, date) => {
  id,
  start: date,
  duration: 1.,
  lunch: 0.,
  userid: Some("testerman"),
};

let dateHelp = makeWithYMDHM(~year=2018., ~month=6., ~hours=8., ~minutes=0.);

test("workListToCurrentWeek", (_) => {
  let testDate =
    makeWithYMDHM(
      ~year=2018.,
      ~month=6.,
      ~date=19.,
      ~hours=14.,
      ~minutes=0.,
      (),
    );
  let list = [
    createWorkOnDate(Some("1"), dateHelp(~date=14., ())),
    createWorkOnDate(Some("2"), dateHelp(~date=15., ())),
    createWorkOnDate(Some("3"), dateHelp(~date=16., ())),
    createWorkOnDate(Some("4"), dateHelp(~date=17., ())),
    createWorkOnDate(Some("5"), dateHelp(~date=18., ())),
    createWorkOnDate(Some("6"), dateHelp(~date=19., ())),
    createWorkOnDate(Some("7"), dateHelp(~date=20., ())),
    createWorkOnDate(Some("8"), dateHelp(~date=21., ())),
    createWorkOnDate(Some("9"), dateHelp(~date=22., ())),
  ];
  Expect.(
    expect(list |> WorkList.workListToCurrentWeek(testDate) |> List.length)
    |> toBe(7)
  );
});