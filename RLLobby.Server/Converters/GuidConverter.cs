using System.Buffers.Text;
using System.Text.Json.Serialization;
using System.Text.Json;

namespace RLLobby.Server.Converters;

public class GuidConverter : JsonConverter<Guid>
{
    public override Guid Read(ref Utf8JsonReader reader, Type type, JsonSerializerOptions options)
    {
        if (reader.TokenType == JsonTokenType.String)
        {
            var span = reader.ValueSpan;

            if (Utf8Parser.TryParse(span, out Guid guid, out int bytesConsumed) && span.Length == bytesConsumed)
                return guid;

            if (Guid.TryParse(reader.GetString(), out guid))
                return guid;
        }
        return Guid.Empty;
    }

    public override void Write(Utf8JsonWriter writer, Guid value, JsonSerializerOptions options)
        => writer.WriteStringValue(value.ToString());
}