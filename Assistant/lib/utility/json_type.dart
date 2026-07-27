import '/common.dart';
import '/utility/convert_helper.dart';

// ----------------

typedef JsonNode = Object?;

typedef JsonArray = List<JsonNode>;

typedef JsonObject = Map<String, JsonNode>;

// ----------------

extension DynamicJsonExtension on JsonNode {

  Null jsonNull(
  ) {
    return this?.as();
  }

  Boolean jsonBoolean(
  ) {
    return this!.as();
  }

  Integer jsonInteger(
  ) {
    return this!.as();
  }

  Floater jsonFloater(
  ) {
    return this!.as();
  }

  String jsonString(
  ) {
    return this!.as();
  }

  JsonArray jsonArray(
  ) {
    return this!.as();
  }

  JsonObject jsonObject(
  ) {
    return this!.as();
  }

  // ----------------

  JsonNode jsonAt(
    Integer index,
  ) {
    return this.jsonArray()[index];
  }

  JsonNode jsonIn(
    String key,
  ) {
    assertTest(this.jsonObject().containsKey(key));
    return this.jsonObject()[key];
  }

  JsonNode jsonInOr(
    String key,
  ) {
    return this.jsonObject()[key];
  }

  // ----------------

  String jsonKey(
  ) {
    var original = this!.as<String>();
    return ConvertHelper.changeStringFromCamelCaseToSnakeCase(original);
  }

}
