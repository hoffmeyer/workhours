open Jest;

open Types;

open Js.Date;

let createWorkOnDate = (id, date) => {
  id,
  start: date,
  duration: 1.,
  lunch: 0.,
  userid: "testerman",
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
    createWorkOnDate("1", dateHelp(~date=14., ())),
    createWorkOnDate("2", dateHelp(~date=15., ())),
    createWorkOnDate("3", dateHelp(~date=16., ())),
    createWorkOnDate("4", dateHelp(~date=17., ())),
    createWorkOnDate("5", dateHelp(~date=18., ())),
    createWorkOnDate("6", dateHelp(~date=19., ())),
    createWorkOnDate("7", dateHelp(~date=20., ())),
    createWorkOnDate("8", dateHelp(~date=21., ())),
    createWorkOnDate("9", dateHelp(~date=22., ())),
  ];
  Expect.(
    expect(list |> WorkList.workListToCurrentWeek(testDate) |> List.length)
    |> toBe(7)
  );
});