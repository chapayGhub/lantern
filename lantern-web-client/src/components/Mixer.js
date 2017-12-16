import xs from 'xstream'
// import isolate from '@cycle/isolate'
import {div, ul, button} from '@cycle/dom';
import Channel from './Channel'
import { makeCollection } from 'cycle-onionify';
import isolate from '@cycle/isolate';
import throttle from 'xstream/extra/throttle'

function intent(domSource) {
    return {
        requests$: domSource.select('.request-state').events('click').map(() => ({ command: 'sendState' }))
    }
}

function model(actions, sources) {
    const initMixerReducer$ = xs.of(() => { return { channels: [] } })
    const mixerReducer$ = xs.combine(sources.server).map(([serverState, channelSink]) => { return (prevState) => {
        const channels = serverState.channels.map((serverChannel, i) => { 
            let prevStateChannel = {}
            if (prevState.channels && prevState.channels[i]) {
                prevStateChannel = prevState.channels[i]
            }
            return {
                ...prevStateChannel,
                ...serverChannel,
                channelIndex: i
            }
        })
        const result = {
            ...serverState,
            channels
        }
        return result
    }})
    const reducer$ = xs.merge(initMixerReducer$, mixerReducer$)
    return reducer$
}

function view(state$, channelDOM$) {
    return xs.combine(state$, channelDOM$).map(([mixerState, channelVNodes]) => {
        return (
            <div className="server-state">
                {/* <button className="request-state">Request State</button> */}
                {channelVNodes}
            </div>
        )
    })
}

function ChannelList(sources) {
    const ChannelCollection = makeCollection({
        item: Channel,
        itemKey: (childState, index) => index,//childState.effect.id,
        itemScope: key => key,
        collectSinks: instances => {
            return {
                onion: instances.pickMerge('onion'),
                DOM: instances.pickCombine('DOM').map(itemVNodes => (ul('.channels', itemVNodes))),
                server: instances.pickMerge('server')
            }
        }
    })
    const channelSinks = ChannelCollection(sources)

    return channelSinks
}

export default function(sources) {
    const channelSinks = isolate(ChannelList, 'channels')(sources)
    const actions = intent(sources.DOM)
    const mixerReducer$ = model(actions, sources)
    const vdom$ = view(sources.onion.state$, channelSinks.DOM)
    const initialRequest$ = xs.of({ command: 'sendState' })

    const sendState$ = channelSinks.server.mapTo({ command: 'sendState' }).compose(throttle(60))
    const serverSink$ = xs.merge(initialRequest$, actions.requests$, channelSinks.server, sendState$)

    return {
        onion: xs.merge(mixerReducer$, channelSinks.onion),
        DOM: vdom$,
        server: serverSink$
    }
}