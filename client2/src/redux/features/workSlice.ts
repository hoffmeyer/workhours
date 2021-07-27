import { createSlice, PayloadAction } from '@reduxjs/toolkit'
import type { RootState } from '../store'
import {Work} from '../../../../src/types'

type WorkState = {
    work: Work[]
}

const initialState: WorkState = {
    work: []
}

export const workSlice = createSlice({
    name: 'work',
    initialState,
    reducers: {
    }
})

export const {} = workSlice.actions
// Other code such as selectors can use the imported `RootState` typeexport const selectCount = (state: RootState) => state.counter.value
export default workSlice.reducer