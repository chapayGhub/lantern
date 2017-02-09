//
//  Effect.swift
//  Lantern Client
//
//  Created by Eric Miller on 2/6/17.
//  Copyright © 2017 patternleaf LLC. All rights reserved.
//

import Foundation
import Freddy
import RxSwift
import RxCocoa

class Effect: Model, JSONDecodable, Equatable {
	
	var id: String?
	var name: Variable<String> = Variable<String>("")
	var parameters: Variable<[EffectParameter]> = Variable<[EffectParameter]>([])
	
	required init(json: JSON) throws {
		id = try json.getString(at: "id")
		if id == "unknown-effect" {
			name.value = "Unknown Effect"
		}
		else {
			name.value = try json.getString(at: "name")
			let jsonParameters = try json.getArray(at: "parameters")
			for parameterJson in jsonParameters {
				parameters.value.append(try ParameterType.create(json: parameterJson))
			}
		}
	}
	
	override func update(json: JSON) throws {
		if id == "unknown-effect" {
			name.value = "Unknown Effect"
		}
		else {
			name.value = try json.getString(at: "name")
			let jsonParameters = try json.getArray(at: "parameters")
			if jsonParameters.count != parameters.value.count {
				throw Model.ErrorType.mismatchedParameterCount(name.value, jsonParameters.count, parameters.value.count)
			}
			var i: Int = 0
			for parameterJson in jsonParameters {
				let incomingId = try parameterJson.getString(at: "id")
				let existingParam = parameters.value[i]
				if incomingId != existingParam.id {
					throw Model.ErrorType.mismatchedParameterId(name.value, try parameterJson.getString(at: "name"))
				}
				try existingParam.update(json: parameterJson)
				i += 1
			}
		}
	}
	
	static func ==(lhs: Effect, rhs: Effect) -> Bool {
		if let lid = lhs.id, let rid = rhs.id {
			return lid == rid
		}
		return false
	}
}

