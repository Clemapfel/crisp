float project(float lower_bound, float upper_bound, float value)
{
    return value * abs(lower_bound - upper_bound) + min(lower_bound, upper_bound);
}

vec2 project(vec2 lower_bound, vec2 upper_bound, vec2 value)
{
    return value * abs(lower_bound - upper_bound) + min(lower_bound, upper_bound);
}

vec3 project(vec3 lower_bound, vec3 upper_bound, vec3 value)
{
    return value * abs(lower_bound - upper_bound) + min(lower_bound, upper_bound);
}

vec4 project(vec4 lower_bound, vec4 upper_bound, vec4 value)
{
    return value * abs(lower_bound - upper_bound) + min(lower_bound, upper_bound);
}